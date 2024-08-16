#include "SoundProcessor.h"
#include "WAVFileWriter.h"

#include <iostream>

#include <memory>  // Для std::unique_ptr

SoundProcessor::SoundProcessor(const std::string& output_file, const std::vector<std::string>& input_files, const std::string& config_file)
    : output_file(output_file), input_files(input_files), config_file(config_file) {}

void SoundProcessor::MainProcess() {
    std::vector<std::vector<int16_t>> input_streams; // Вектор для хранения данных (тоже векторов) всех входных WAV-файлов
    int sample_rate = 0; // expect to be same for every WAV file
    int num_samples = 0;

    // 1 REad all WAV files (samples) and store them in input_streams
    std::cout << "-----1 READ ALL WAV FILES-----" << std::endl;
    bool is_first_file = true;
    for (const auto& input_file : input_files) {
        WAVFileReader reader(input_file);
        input_streams.push_back(reader.ReadSamples());

        if (is_first_file) {
            sample_rate = reader.GetSampleRate();
            num_samples = reader.GetNumSamples();
            is_first_file = false;
        }
    }

    std::cout << "\nAmount of input samples for output: " << num_samples << std::endl;
    std::cout << "Input sample rate: " << sample_rate << std::endl;

    // 2 Parsing configuration file and ctreate converters
    std::cout << "\n-----2 PARSE CONFIGURATION FILE-----" << std::endl;
    ConfigParser parser(config_file, input_files);
    parser.Parse(); // read line by line and create required converters
    const std::vector<std::unique_ptr<Converter>>& converters = parser.GetConverters();

    // 3 Apply converters
    std::cout << "\n-----3 APPLY CONVERTERS-----" << std::endl;
    std::vector<int16_t> processed_samples;
    std::vector<int16_t> output = input_streams[0]; // first input WAV file - the output of the first converter

    std::cout << "START PROCESSING SAMPLES..." << std::endl;
    for (const std::unique_ptr<Converter>& converter : converters) {
        processed_samples = converter->Convert(output);
        output = processed_samples; // apply converters to same output, one by one, changing output
    }
    std::cout << "\nEND PROCESSING SAMPLES" << std::endl;

    std::cout << "\nAmount of processed samples: " << processed_samples.size() << std::endl;
    
    // 4 Save output WAV file
    std::cout << "\n-----4 SAVE OUTPUT WAV FILE-----" << std::endl;
    WAVFileWriter writer(output_file, sample_rate, num_samples);
    writer.WriteSamples(output); // write converted samples to output WAV file
}
