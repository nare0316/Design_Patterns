#include <iostream>
#include <fstream>
#include <memory>

class DataSource {
public:
    virtual void writeData(std::string& data) = 0;
    virtual std::string readData() = 0;
};

class FileDataSource : public DataSource {
    std::string& filename;
public:
    FileDataSource(std::string& filename) : filename(filename) { }
    void writeData(std::string& data) override  {
        std::ofstream infile(filename);
        if (!infile) {
            std::cerr << "failed file opening" << std::endl;
            exit(EXIT_FAILURE);
        }
        infile << data;
        infile.close();
    }

    std::string readData() override {
        std::ifstream outfile(filename);
        if (!outfile) {
            std::cerr << "Failed file open" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::string data;
        std::string line;
        while (std::getline(outfile, line)) {
            data += line;
        }
        outfile.close();
        return data;
    }
};


class DataSourceDecorator : public DataSource {
protected:
    DataSource& wrappee;
public:
    DataSourceDecorator(DataSource& source) : wrappee(source) {}

    void writeData(std::string& data) {
        wrappee.writeData(data);
        std::cout << "Data successfully written in the file" << std::endl;
    }

    std::string readData() {
        return wrappee.readData();
        std::cout << "Data successfully read from the file" << std::endl;
    }
};

class EncryptionDecorator : public DataSourceDecorator {
    void encrypt(std::string& data) {
        for (auto& sym : data) {
            if (sym >= 'a' && sym <= 'z') {
                sym = (sym - 'a' + 3) % 26 + 'a';
            }
        }
    }
    void decrypt(bool isEncrypted, std::string& data) {
        if (!isEncrypted) {
            return;
        }
        for (auto& sym : data) {
            if (sym >= 'a' && sym <= 'z') {
                sym = (sym - 'a' - 3 + 26) % 26 + 'a'; 
            }
        }
    }
public:
    EncryptionDecorator(DataSource& source) : DataSourceDecorator(source) { } 
    void writeData(std::string& data)  {
        encrypt(data);
        wrappee.writeData(data);
    }

    std::string readData() {
       std::string data = wrappee.readData();
       decrypt(true, data);
       return data;
    }
};

class CompressionDecorator : public DataSourceDecorator {
public:
    CompressionDecorator(DataSource& source) : DataSourceDecorator(source) { }
    void writeData(std::string& data) {
        size_t size = data.size();
        std::string compressedData = "";
        size_t i = 0;
        size_t count = 1;
        for (size_t i{}; i < size - 1; ) {
            if (data[i] == ' ') {
                compressedData += data[i];
                compressedData += std::to_string(count);
                ++i;
                continue;
            }
            if (data[i] == data[i+1]) {
                while (data[i] == data[i+1]) {
                    ++count;
                    ++i;
                }
                compressedData += data[i];
                compressedData += std::to_string(count);
                count = 1;
            } else {
                ++i;
            }
        }
        compressedData += data[size - 1];
        compressedData += std::to_string(count);
        wrappee.writeData(compressedData);
    }
    std::string readData() {
        std::string data = wrappee.readData();
        std::string deCompressedData = "";
        size_t size = data.size();
        for (size_t i{1}; i < size; ) {
            if (data[i] >= '1' && data[i] <= '9') {
                size_t j = i;
                std::string count_str = "";
                while (data[j] >= '1' && data[j] <= '9') {
                    count_str += data[j];
                    ++j;
                }
                size_t count_num = std::stoull(count_str);
                for (size_t k{}; k < count_num; ++k) {
                    deCompressedData += data[i - 1];
                }
                i = ++j; 
            } else {
                ++i;
            }
        }
        return deCompressedData;
    }
};

class Application {
public:
    Application() = default;
    void dumbUsageExample() {
        std::string filename = "file.txt";
        std::unique_ptr<FileDataSource> source = std::make_unique<FileDataSource>(filename);
        std::string salaryRecords = "aaaaaaaaaaaaaaaaaaaaa bb ccc d k";
        source->writeData(salaryRecords);
        std::unique_ptr<EncryptionDecorator> encript_source = std::make_unique<EncryptionDecorator>(*source);
        encript_source->writeData(salaryRecords);
        std::unique_ptr<CompressionDecorator> compress_source = std::make_unique<CompressionDecorator>(*source);
        compress_source->writeData(salaryRecords);
    }
};

int main() {
    Application *app = new Application;
    app->dumbUsageExample();
    
    // std::string filename = "file.txt";
    // std::string write_data = "aaaaaaaaaaaaaaaaaaaaa bb ccc d k";
    // FileDataSource file_data(filename);

    // EncryptionDecorator encrypt_data(file_data);
    // encrypt_data.writeData(write_data);
    // std::string data = encrypt_data.readData();
    // std::cout << data << std::endl;

    // CompressionDecorator compress_data(file_data);
    // compress_data.writeData(write_data);
    // std::string data = compress_data.readData();
    // std::cout << data << std::endl;
}