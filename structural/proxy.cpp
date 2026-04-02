#include <iostream>
#include <exception>
#include <memory>

class InvalidVideo : public std::runtime_error {
public:
    InvalidVideo(std::string err) : std::runtime_error(err) {}
};
class Video {
public:
    static int id;
    int ID;
    std::string name;
    std::string info;
public:
    Video(std::string name) : name(name), info("no info") {
        ID = id++;
    }

    void set_info(std::string info) {
        this->info = info;
    }

    std::string get_info() const {
        return info;
    }
    friend std::ostream& operator<<(std::ostream& os, const Video& video) {
        os << "Video name: " << video.name << ". Video id: " << video.ID << std::endl;
        return os;
    }
};
int Video::id{};

// The interface of a remote service.
class ThirdPartyYouTubeLib {
public:
    virtual std::vector<Video> listVideos() = 0;
    virtual void add_video(Video& video) = 0;
    virtual std::string getVideoInfo(int id) = 0;
    virtual Video downloadVideo(int id) = 0;
    virtual ~ThirdPartyYouTubeLib() {};
};
// The concrete implementation of a service connector. Methods
// of this class can request information from YouTube. The speed
// of the request depends on a user's internet connection as
// well as YouTube's. The application will slow down if a lot of
// requests are fired at the same time, even if they all request
// the same information.
class ThirdPartyYouTubeClass : public ThirdPartyYouTubeLib {
public:
    std::vector<Video> videos;

    std::vector<Video> listVideos() override {
        // Send an API request to YouTube.
        return videos;
    }

    void add_video(Video& video) override {
        videos.push_back(video);
    }

    int find_video(int id) {
        for(size_t i{}; i < videos.size(); ++i) {
            if (videos[i].ID == id) {
                return i;
            }
        }
        return -1;
    }
    std::string getVideoInfo(int id) override {
        // Get metadata about some video.
        int index = find_video(id);
        if (index != -1) {
            return videos[index].get_info();
        } 
        return "No such a video";
    }

    Video downloadVideo(int id) override {
        // Download a video file from YouTube.
        int index = find_video(id);
        if (index == -1) {
            throw InvalidVideo("No such a video");
        }
        return videos[index];
    }
        
};
// To save some bandwidth, we can cache request results and keep
// them for some time. But it may be impossible to put such code
// directly into the service class. For example, it could have
// been provided as part of a third party library and/or defined
// as `final`. That's why we put the caching code into a new
// proxy class which implements the same interface as the
// service class. It delegates to the service object only when
// the real requests have to be sent.
class CachedYouTubeClass : public ThirdPartyYouTubeLib {
private:
    ThirdPartyYouTubeLib& service;
    std::vector<Video> listCache;
    std::string videoCache;
public:
    CachedYouTubeClass(ThirdPartyYouTubeLib& service) : service(service), videoCache("") {}

    std::vector<Video> listVideos() override {
        if (listCache.empty()) {
            listCache = service.listVideos();
        }
        return listCache;
    }

    std::string getVideoInfo(int id) override {
        if (videoCache == "") {
            videoCache = service.getVideoInfo(id);
        }
        return videoCache;
    }

    Video downloadVideo(int id) override {
        return service.downloadVideo(id);
    }

    void add_video(Video& video) override {
        service.add_video(video);
    }
};
// The GUI class, which used to work directly with a service
// object, stays unchanged as long as it works with the service
// object through an interface. We can safely pass a proxy
// object instead of a real service object since they both
// implement the same interface.
class YouTubeManager {
protected:
    ThirdPartyYouTubeLib& service;
public:
    YouTubeManager(ThirdPartyYouTubeLib& service) : service(service) {}

    void renderListPanel() {
        // Render the list of video thumbnails.
        std::vector<Video> videos = service.listVideos();
        for (const auto& video : videos) {
            std::cout << video;
        }
    }

    void reactOnUserInput() {
        renderListPanel();
    }
};
// The application can configure proxies on the fly.
class Application {
public:
    void init() {
        Video video("kargin");
        video.set_info("10 Kb");
        Video video2("Ejanish");
        video2.set_info("54 Kb");
        
        std::unique_ptr<ThirdPartyYouTubeLib> aYouTubeService = std::make_unique<ThirdPartyYouTubeClass>();
        std::unique_ptr<ThirdPartyYouTubeLib> aYouTubeProxy = std::make_unique<CachedYouTubeClass>(*aYouTubeService);
        aYouTubeProxy->add_video(video);
        aYouTubeProxy->add_video(video2);
        std::unique_ptr<YouTubeManager> manager = std::make_unique<YouTubeManager>(*aYouTubeProxy);
        manager->reactOnUserInput();
    }
};

int main() {
    Application app;
    app.init();
}