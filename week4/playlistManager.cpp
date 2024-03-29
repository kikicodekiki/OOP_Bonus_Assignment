#include <iostream>
#include <fstream>
#include <cstring>

namespace GlobalConstants {
    constexpr size_t NAME_MAX_SIZE = 64;
    constexpr size_t MAX_BYTES = 256;
    constexpr size_t MAX_SONGS_IN_PLAYLIST = 30;
    constexpr size_t MAX_NUM_GENRES = 6; //+1 for '\0'
    constexpr size_t SECONDS_IN_HOUR = 3600;
    constexpr size_t SECONDS_IN_MINUTE = 60;
    constexpr size_t SECONDS_IN_DAY = 24*3600;
}

typedef char contentInBytes[GlobalConstants::MAX_BYTES];

enum class Genre : char {
    ROCK = 1,
    POP =  1 << 1,
    HIP_HOP = 1 << 2,
    ELECTRONIC = 1 << 3,
    JAZZ = 1 << 4,
    Unknown = 0
};

//straight from dimitriev's github :)
class Time {
private:
    unsigned seconds = 0;

    bool validate (unsigned from, unsigned to,
                   unsigned newValue, unsigned oldValue,
                   unsigned multiplier) {
        if (newValue >= from && newValue <= to) {
            (seconds -= (oldValue * multiplier)) += (newValue * multiplier);
            return true;
        }
        else {
            return false;
        }
    }
public:
    Time() = default;
    Time (unsigned hours, unsigned minutes, unsigned seconds);
    unsigned  getHours () const;
    unsigned getMinutes () const;
    unsigned getSeconds () const;
    unsigned getDuration () const;
    bool setHours (unsigned hours);
    bool setMinutes (unsigned minutes);
    bool setSeconds (unsigned seconds);
    void incrementSeconds () ;
    void print () const;
    bool setDuration (const Time& newDuration);
};

Time::Time(unsigned int hours, unsigned int minutes, unsigned int seconds) {
    setHours(hours);
    setMinutes(minutes);
    setSeconds(seconds);
}

unsigned Time::getHours() const {
    return seconds / GlobalConstants::SECONDS_IN_HOUR;
}

unsigned Time::getMinutes() const {
    return (seconds/GlobalConstants::SECONDS_IN_MINUTE)%GlobalConstants::SECONDS_IN_MINUTE;
}

unsigned Time::getSeconds () const {
    return seconds%GlobalConstants::SECONDS_IN_MINUTE;
}

unsigned Time::getDuration() const {
    return this->seconds;
}

bool Time::setHours(unsigned hours)
{
    return validate(0, 23, hours,
                    getHours(), GlobalConstants::SECONDS_IN_HOUR);
}

bool Time::setMinutes(unsigned int minutes)
{
    return validate(0, 59, minutes,
                    getMinutes(), GlobalConstants::SECONDS_IN_MINUTE);

}
bool Time::setSeconds(unsigned seconds)
{
    return validate(0, 59, seconds,
                    getSeconds(), 1);
}

void Time::incrementSeconds() {
    ++seconds %= GlobalConstants::SECONDS_IN_DAY;
}

void Time::print() const {
    std :: cout << getHours() << ":"
                << getMinutes()
                << ":" << getSeconds();
}


class Song {
public:
    char name[GlobalConstants::NAME_MAX_SIZE + 1] = "Unknown Song";
    Time duration; //seconds
    int genre = (int)(Genre::Unknown);
    contentInBytes content = " ";
public:

    Song () = default;
    void setName (const char* newName);
    void  setDuration (const Time& newDuration);
    void setGenre (const int newGenre);
    void setContentFromBinaryFile (const char* fileName);
    void setContent (const contentInBytes newContent);

    const char* getName () const;
    const Time& getDuration () const;
    int getGenre () const;
    size_t getContentLen () const ;
    const char* getContent () const;
    void mix (Song& to, const Song& from);
 };

void Song::setName(const char *newName) {
    if (!newName) {
        return;
    }
    if (strlen(newName) > GlobalConstants::NAME_MAX_SIZE) {
        return;
    }
    strncpy(name, newName, GlobalConstants::NAME_MAX_SIZE);
    name[GlobalConstants::NAME_MAX_SIZE] = '\0';
}

void Song::setDuration(const Time& newDuration)  {
    duration = newDuration;
}

void Song::setGenre(const int newGenre) {
    genre = newGenre;
}

void Song ::setContentFromBinaryFile(const char* fileName) {

    std::fstream ifs (fileName, std::ios::in|std::ios::binary);

    if(!ifs.is_open()) {
        std::cerr << "File " << fileName << "not open";
        return;
    }

    ifs.read(content, GlobalConstants::MAX_BYTES);

    ifs.close();

}

void Song::setContent(const char *newContent) {
    if(!newContent) {
        return;
    }
    if(strlen(newContent) > GlobalConstants::MAX_BYTES) {
        return;
    }
    strncpy(content, newContent, GlobalConstants::MAX_BYTES);
}

const char* Song::getName() const {
    return name;
}

const Time& Song::getDuration() const {
    return duration;
}

int Song::getGenre() const {
    return genre;
}

size_t Song::getContentLen() const {
    size_t contentLen = 0;
    size_t idx = 0;

    while ( content[idx] != '\0') {
        contentLen++;
        idx++;
    }

    return contentLen;
}

void Song::mix (Song& to, const Song& from) {
    size_t contentSize = getContentLen();

    for (size_t i = 0; i < contentSize; i ++) {
        to.content[i] ^= from.content[i];
    }
}


class Playlist {
    Song songs[GlobalConstants::MAX_SONGS_IN_PLAYLIST] = {};
    size_t numberOfSongs = 0;

public:
    Playlist() = default;

    bool playlistIsFull () const{
        return numberOfSongs >= GlobalConstants::MAX_SONGS_IN_PLAYLIST;
    }

    void add (const char songName [GlobalConstants::NAME_MAX_SIZE], unsigned hours,
              unsigned minutes, unsigned seconds,
              const char genre[GlobalConstants::MAX_NUM_GENRES],
              const char* fileName);
    void addSongFromFile(const char* fileName) ;
    void print() const;
    const Song& findSong (const char* songName) const;
    void findByGenre (const char* genre) const;
    void sortByDuration ();
    void sortByName ();
    void mix (Song& song1, const Song& song2);
    void writeSongToBinaryFile (const char songName[GlobalConstants::NAME_MAX_SIZE],
                           const char* fileName);
    void save(const char* songName, const char* fileName);
};

int stringToGenre(const char* genreStr) {
    int myGenre = 0;
    for (int i = 0; genreStr[i] != '\0'; i++) {
        char genreChar = genreStr[i];
        switch (genreChar) {
            case 'r':
                myGenre |= (int)(Genre::ROCK);
                break;
            case 'p':
                myGenre |= (int)(Genre::POP);
                break;
            case 'h':
                myGenre |= (int)(Genre::HIP_HOP);
                break;
            case 'e':
                myGenre |= (int)(Genre::ELECTRONIC);
                break;
            case 'j':
                myGenre |= (int)(Genre::JAZZ);
                break;
            default:
                break;
        }
    }
    return myGenre;
}


void Playlist ::add(const char *songName, unsigned hours, unsigned minutes, unsigned seconds,
                    const char *genre, const char *fileName) {

    if (playlistIsFull()) {
        std::cerr << "Playlist is full. Unable to add more songs \n";
        return;
    }
    Song newSong;
    newSong.setName(songName);
    Time duration (hours, minutes, seconds);
    newSong.setDuration(duration);
    int myGenre = stringToGenre(genre);
    newSong.setGenre(myGenre);

    songs[numberOfSongs++] = newSong;
    std::ofstream ofs (fileName, std::ios::app);

    if(!ofs.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    ofs << songName << " " << hours << ":" << minutes << ":"
        << seconds << " " << genre << "\n";
    ofs.close();
}

void Playlist::writeSongToBinaryFile (const char songName[GlobalConstants::NAME_MAX_SIZE],
                            const char* fileName) {
//trqbva mi funckiq za da namerq pesenta po ime v playlista
    std::fstream ofs (fileName, std::ios::binary|std::ios::out);
    //ofs.write((const char*)&)
}

Playlist readPlaylistFromBinaryFile (std::ifstream& in) {
    Playlist playlist;

    in.read((char*)&playlist, sizeof(playlist));

    return playlist;
}

const Song& readSongFromFile ( const char* fileName, Song& song) {
    std::ifstream in (fileName);

    if(!in.is_open()) {
        std::cerr << "File not open";
        return {};
    }

    unsigned int hours, minutes, seconds;

    char genre[GlobalConstants::MAX_NUM_GENRES];
    in >> song.name;
    in >> hours ;
    in.ignore(); // because hours:minutes:seconds ":"
    in >> minutes;
    in.ignore();
    in >> seconds;

    song.duration.setHours(hours);
    song.duration.setMinutes(minutes);
    song.duration.setSeconds(seconds);

    in >> genre;

    int myGenre = stringToGenre(genre);
    song.genre = myGenre;

    in >> song.content;

    return song;
}


void Playlist:: addSongFromFile(const char* fileName) {
    if(playlistIsFull()) {
        std::cerr << "Playlist is full. Unable to add more songs \n";
        return;
    }

    Song newSong;

    songs[numberOfSongs++] = readSongFromFile(fileName, newSong);
}

void printGenre (const Song& mySong) {
    bool firstGenre = true;
    for (int j = 1; j <= (int)(Genre::JAZZ); j <<= 1) {
        if (mySong.getGenre() & j) {
            if (!firstGenre) {
                std::cout << " & ";
            }
            switch (static_cast<Genre>(j)) {
                case Genre::ROCK:
                    std::cout << "Rock";
                    break;
                case Genre::POP:
                    std::cout << "Pop";
                    break;
                case Genre::HIP_HOP:
                    std::cout << "Hip Hop";
                    break;
                case Genre::ELECTRONIC:
                    std::cout << "Electronic";
                    break;
                case Genre::JAZZ:
                    std::cout << "Jazz";
                    break;
                default:
                    std::cout << "Unknown";
                    break;
            }
            firstGenre = false;
        }
    }

    std::cout << std::endl;
}


void Playlist::print() const {
    for (size_t i = 0; i < numberOfSongs; i++) {
        std::cout << songs[i].getName() << ", ";
        songs[i].duration.print();
        std::cout << ", ";

        // Print genres
        printGenre(songs[i]);

    }
}


const Song& Playlist::findSong (const char* songName) const {
    for (size_t i = 0; i < numberOfSongs; i++) {
        if (strcmp(songs[i].name, songName) == 0) {
            std::cout << songName << " ";
            songs[i].duration.print();
            std::cout << " ";
            printGenre(songs[i]);
            return songs[i];
        }
    }
    return {};
}

void Playlist :: findByGenre (const char* genre) const {

    int myGenre = stringToGenre(genre);

    for (size_t i = 0; i < numberOfSongs; i++) {
        if(songs[i].getGenre() & myGenre) {
            std::cout << songs[i].name << ", ";
            songs[i].duration.print();
            std::cout << ", ";
            printGenre(songs[i]);
            std::cout << std::endl;
        }
    }
}

void Playlist ::sortByDuration () {
    //using selection sort
    for (size_t i = 0; i < numberOfSongs - 1; ++i) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < numberOfSongs; ++j) {
            if (songs[j].duration.getDuration() < songs[minIndex].duration.getDuration()) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            std::swap(songs[i], songs[minIndex]);
        }
    }

}

void Playlist::sortByName() {
    for (size_t i = 0; i < numberOfSongs - 1; i++ ) {
        size_t minIndex = i;

        for (size_t j = i + 1; j < numberOfSongs; j++ ) {
            if (strcmp(songs[j].getName(), songs[minIndex].getName()) < 0) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            std::swap (songs[i], songs[minIndex]);
        }
    }
}

void Playlist::mix(Song &song1, const Song &song2) {
    song1.mix(song1, song2);
}

void Playlist::save(const char* songName, const char* fileName) {
    // Find the song by name
    const Song& songToSave = findSong(songName);

    // Open the binary file for writing
    std::ofstream ofs(fileName, std::ios::binary);
    if (!ofs.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    // Write the content of the song to the file
    ofs.write(songToSave.getContent(), songToSave.getContentLen());

    // Close the file
    ofs.close();
}


int main() {

    Playlist p;
    p.add("Song 2", 7, 3, 55, "pr", "song2.dat");
    p.add("Song 1", 0, 1, 5, "rj", "song1.dat");

    //p.findSong("Song 1");

    //p.findByGenre("r");

    //p.sortByDuration();
    //p.print();

    //p.save("Song 1", "song3.dat");

    //p.print();

    return 0;

}


