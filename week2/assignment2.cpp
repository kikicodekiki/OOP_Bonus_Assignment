#include<iostream>
#include<cstring>
#include<fstream>

enum class ErrorInCatalog{
    catalog_not_open,
    read_from_empty_catalog,
    movie_not_in_catalog,
    no_error_occurred
};

struct SafeAnswer{
    unsigned int number;
    ErrorInCatalog error;
};

struct Movie{
    char name[128];
    unsigned int price;
};

int getCharCountInFile (std::ifstream& file, char ch) {

    if(!file.is_open()) {
        std::cout << "not open";
        return -1;
    }

    int count = 0;

    while (true) {

        char cur = file.get();
        if(file.eof()) {
            break;
        }

        if (cur == ch)
            count++;

    }
    return count;
}

unsigned int getLinesCount (const char* fileName) {
    std :: ifstream myFile (fileName);

    if (!myFile.is_open()) {
        std:: cout << "not open";
        return -1;
    }

    return getCharCountInFile(myFile, '\n') + 1;
}

SafeAnswer getNumberOfMovies(const char* catalogName){
    SafeAnswer answer;
    answer.error = ErrorInCatalog::no_error_occurred;

    unsigned int size = getLinesCount (catalogName);

    std::ifstream myCatalog (catalogName);

    if (!myCatalog.is_open()) {
        std:: cout << "File not open.";
        answer.error = ErrorInCatalog::catalog_not_open;
        return answer;
    }

    answer.number = size;

    return answer;
}

SafeAnswer averagePrice(const char* catalogName){
    SafeAnswer answer;
    answer.error = ErrorInCatalog::no_error_occurred;
    unsigned int size = getLinesCount (catalogName);

    std:: ifstream myCatalog (catalogName);
    if (!myCatalog.is_open()) {
        std:: cout << "File not open.";
        answer.error = ErrorInCatalog::catalog_not_open;
        return answer;
    }

    if ( size == 0 ) {
        std::cout << "empty file";
        answer.error = ErrorInCatalog::read_from_empty_catalog;
        return answer;
    }

    Movie myMovie;

    unsigned int combinedPrice = 0;

    while (!myCatalog.eof()) {
        myCatalog >> myMovie.name >> myMovie.price;

        combinedPrice += myMovie.price;
    }

    answer.number = (combinedPrice/size);

    return answer;
}

SafeAnswer getMoviePrice(const char* catalogName, const char* movieName){
    SafeAnswer answer;
    answer.error = ErrorInCatalog::movie_not_in_catalog;
    unsigned int size = getLinesCount (catalogName);

    std:: ifstream myCatalog (catalogName);
    if (!myCatalog.is_open()) {
        std:: cout << "File not open.";
        answer.error = ErrorInCatalog::catalog_not_open;
        return answer;
    }

    if ( size <= 0 ) {
        std::cout << "empty file";
        answer.error = ErrorInCatalog::read_from_empty_catalog;
        return answer;
    }

    Movie myMovie;

    while (!myCatalog.eof()) {

        myCatalog >> myMovie.name >> myMovie.price;

        if (strcmp(myMovie.name, movieName) == 0) {
            answer.error = ErrorInCatalog::no_error_occurred;
            answer.number = myMovie.price;
            break;
        }
    }

    return answer;
}

Movie readMovie(std::ifstream& file){

    Movie movie;
    if(!file.is_open()) {
        std::cout << "not open";
        return {};
    }

    file >> movie.name >> movie.price;

    return movie;
}

Movie* saveMoviesInArray(std::ifstream& file, unsigned int numberOfMovies){

    if(!file.is_open()) {
        std::cout << "not open";
        return nullptr;
    }

    Movie* moviesArr = new Movie [numberOfMovies];

    unsigned int i = 0;
    while (!file.eof() || i < numberOfMovies) {
        moviesArr[i] = readMovie(file);
        i++;
    }

    return moviesArr;
}

void freeMoviesFromArray(Movie*& moviesArray){

    if (moviesArray != nullptr) {
        delete[] moviesArray;
        moviesArray = nullptr;
    }

}

void sortMoviesInArray(Movie*& moviesArray, unsigned int numberOfMovies){

    for (unsigned int i = 0; i < numberOfMovies - 1; i++) {

        unsigned int minIndex = i;

        for (unsigned int j = i+1; j < numberOfMovies; j++) {
            if(moviesArray[j].price < moviesArray[minIndex].price ) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            std::swap(moviesArray[i].price, moviesArray[minIndex].price );
        }

    }


}

ErrorInCatalog saveMoviesSorted(const char* catalogName, const char* catalogSortedName){

    SafeAnswer numberOfMovies = getNumberOfMovies(catalogName);

    std::ifstream myCatalog(catalogName);
    if (!myCatalog.is_open()) {
        std::cout << "File not open.";
        return ErrorInCatalog::catalog_not_open;
    }

    std::ofstream sortedCatalog(catalogSortedName);
    if (!sortedCatalog.is_open()) {
        std::cout << "File not open.";
        return ErrorInCatalog::catalog_not_open;
    }

    if (numberOfMovies.error != ErrorInCatalog::no_error_occurred || numberOfMovies.number == 0) {
        std::cout << "No movies in the catalog.";
        return ErrorInCatalog::read_from_empty_catalog;
    }

    Movie* moviesArr = saveMoviesInArray(myCatalog, numberOfMovies.number);
    sortMoviesInArray(moviesArr, numberOfMovies.number);

    for (int i = 0; i < numberOfMovies.number; i++) {
        sortedCatalog << moviesArr[i].name << " \t " << moviesArr[i].price << std::endl;
    }

    sortedCatalog.close();
    freeMoviesFromArray(moviesArr);

    return ErrorInCatalog::no_error_occurred;
}

int main(){
    SafeAnswer safeNumberOfMovies = getNumberOfMovies("movieCatalog.txt");
    if(safeNumberOfMovies.error == ErrorInCatalog::no_error_occurred){
        std::cout<<"The number of movies is: "<< safeNumberOfMovies.number << std::endl;
    }
    SafeAnswer safeAveragePrice = averagePrice("movieCatalog.txt");
    if(safeAveragePrice.error == ErrorInCatalog::no_error_occurred){
        std::cout<<"The average price is: "<< safeAveragePrice.number << std::endl;
    }

    SafeAnswer safePrice = getMoviePrice("movieCatalog.txt", "Black-bullet");
    if(safePrice.error == ErrorInCatalog::no_error_occurred){
        std::cout<<"The price for the Black bullet movies is: " << safePrice.number << std::endl;
    }

    ErrorInCatalog errorSorting = saveMoviesSorted("movieCatalog.txt", "movieCatalogSorted.txt");
    if(errorSorting == ErrorInCatalog::no_error_occurred){
        std::cout<<"Look the content of the movieCatalogSorted.txt file" << std::endl;
    }
}
