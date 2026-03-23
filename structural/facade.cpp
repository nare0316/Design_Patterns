#include <iostream>
#include <memory>
#include <fstream>

class VideoFile {
public:
    std::string filename;
    VideoFile(std::string& filename) : filename(filename) {}
    
};

class Format {
public:
    Format() = default;
    virtual ~Format() = default;
    virtual std::string get_format() const = 0;
};
class OggCompressionCodec : public Format {
public:
    OggCompressionCodec() = default;
    virtual std::string get_format() const override {
        return "OggCompress";
    }
};

class MPEG4CompressionCodec : public Format {
public:
    MPEG4CompressionCodec() = default;
    virtual std::string get_format() const override {
        return "MPEG4Compress";
    }
};

class CodecFactory {
public:
    CodecFactory() = default;
    std::string extract(VideoFile& file) {
        size_t j = (file.filename).size() - 1;
        while (j) {
            if (file.filename[j] == '.') {
                break;
            }
            --j;
        }
        std::string exten = file.filename.substr(j, file.filename.size() - j);
        return exten;
    }
};

class BitrateReader {
public:
    std::string read(std::string& filename, std::string& sourceCodec) {
        std::string read_data = " ";
        if (sourceCodec == "txt") {
            std::ifstream file(filename);
            if (!file) {
                std::cerr << "Fail open failed" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::string line;
            while (std::getline(file, line)) {
                read_data += line;
            }
        } else {
            //some logic
        }
        return read_data;
    }

    std::string convert(std::string buffer, Format& destinationCodec) {
        if (destinationCodec.get_format() == "OggCompress") {
            // some logic;
        } else if (destinationCodec.get_format() == "MPEG4Compress") {
            // some logic;
        } else {
            std::cerr << "No such a format" << std::endl;
            exit(EXIT_FAILURE);
        }
        return buffer;
    }
};

class AudioMixer {
std::string some_info;
public:
    AudioMixer() = default;
    std::string fix(std::string& text) {
        std::string result = some_info + text;
        return result;
    }
};

class File {
public:
    std::string filename;
    File(std::string& filename) : filename(filename) {}
};

class VideoConverter {
public:
    std::unique_ptr<File> convert(std::string& filename, std::string& format) {
        std::unique_ptr<VideoFile> file = std::make_unique<VideoFile>(filename);
        std::unique_ptr<CodecFactory> factory = std::make_unique<CodecFactory>();
        std::string sourceCodec = factory->extract(*file);
        std::unique_ptr<Format> destinationCodec;
        if (format == "mp4") {
            destinationCodec = std::make_unique<MPEG4CompressionCodec>();
        }else {
            destinationCodec = std::make_unique<OggCompressionCodec>();
        }
        
        std::string buffer = std::make_unique<BitrateReader>()->read(filename, sourceCodec);
        std::string result = std::make_unique<BitrateReader>()->convert(buffer, *destinationCodec);
        std::string result2 = std::make_unique<AudioMixer>()->fix(result);
        return std::make_unique<File>(result2);
    }
};

class Application {
public:
    Application() = default;
    void run() {
        std::unique_ptr<VideoConverter> convertor = std::make_unique<VideoConverter>();
        std::string filename = "funny-cats-video.ogg";
        std::string format = "mp4";
        std::unique_ptr<File> mp4 = convertor->convert(filename, format);
    }
};

int main() {
    Application app;
    app.run();
}
