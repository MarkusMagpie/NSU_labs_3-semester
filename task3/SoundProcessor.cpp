#include "SoundProcessor.h"
#include "WAVFileWriter.h"

#include <memory>  // Для std::unique_ptr

SoundProcessor::SoundProcessor(const std::string& output_file, const std::vector<std::string>& input_files, const std::string& config_file)
    : output_file(output_file), input_files(input_files), config_file(config_file) {}

void SoundProcessor::MainProcess() {
    std::vector<std::vector<int16_t>> input_streams; // Вектор для хранения данных (тоже векторов) всех входных WAV-файлов
    int sample_rate = 0; // expect to be same for every WAV file
    int num_samples = 0;

    // 1 REad all WAV files (samples) and store them in input_streams
    for (const auto& input_file : input_files) {
        WAVFileReader reader(input_file);
        input_streams.push_back(reader.ReadSamples());
        sample_rate = reader.GetSampleRate();
        num_samples = reader.GetNumSamples();
    }

    // 2 Parsing configuration file and ctreate converters
    ConfigParser parser(config_file, input_files);
    parser.Parse(); // read line by line and create required converters
    const std::vector<std::unique_ptr<Converter>>& converters = parser.GetConverters();
    // const auto& converters = parser.GetConverters();

    // 3 Apply converters
    std::vector<int16_t> output = input_streams[0]; // first input WAV file - the output of the first converter
    for (const auto& converter : converters) {
        output = converter->Convert(output); // apply converters to same output, one by one, changing output
    }

    WAVFileWriter writer(output_file, sample_rate, num_samples);
    writer.WriteSamples(output); // write converted samples to output WAV file
}
