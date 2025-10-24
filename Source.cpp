#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::swap
#include <numeric>   // For std::accumulate
#include <random>    // For random number generation
#include <sstream>   // For std::ostringstream

// Base class for data analysis
class Analyzer {
protected:
    std::vector<int> data;

public:
    Analyzer(const std::vector<int>& input_data) : data(input_data) {}
    virtual ~Analyzer() = default;
    virtual std::string analyze() = 0;
};

// Function to swap two integers
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Selection Sort algorithm
void selection_sort(int* arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < size; ++j) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(&arr[i], &arr[min_idx]);
        }
    }
}

// Recursive binary search function
bool binary_search_recursive(const int* arr, int search_value, int start_index, int end_index) {
    if (start_index > end_index) {
        return false;
    }

    int mid_index = start_index + (end_index - start_index) / 2;

    if (arr[mid_index] == search_value) {
        return true;
    }
    else if (arr[mid_index] < search_value) {
        return binary_search_recursive(arr, search_value, mid_index + 1, end_index);
    }
    else {
        return binary_search_recursive(arr, search_value, start_index, mid_index - 1);
    }
}

// Helper function for binary search
bool binary_search(const int* arr, int search_value, int size) {
    return binary_search_recursive(arr, search_value, 0, size - 1);
}

// SearchAnalyzer class
class SearchAnalyzer : public Analyzer {
public:
    SearchAnalyzer(std::vector<int> input_data) : Analyzer(input_data) {
        selection_sort(this->data.data(), this->data.size());
    }

    std::string analyze() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 999);

        int found_count = 0;
        for (int i = 0; i < 100; ++i) {
            int random_value = distrib(gen);
            if (binary_search(this->data.data(), random_value, this->data.size())) {
                found_count++;
            }
        }
        std::ostringstream oss;
        oss << "Search Analysis: Found " << found_count << " random values.";
        return oss.str();
    }
};

// StatisticsAnalyzer class
class StatisticsAnalyzer : public Analyzer {
public:
    StatisticsAnalyzer(std::vector<int> input_data) : Analyzer(input_data) {}

    std::string analyze() override {
        if (data.empty()) {
            return "Statistics Analysis: No data to analyze.";
        }

        selection_sort(this->data.data(), this->data.size());

        // Min and Max
        int min_val = data.front();
        int max_val = data.back();

        // Mean
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        double mean = sum / data.size();

        // Median
        double median;
        size_t mid_idx = data.size() / 2;
        if (data.size() % 2 == 1) {
            median = data[mid_idx];
        }
        else {
            median = (static_cast<double>(data[mid_idx - 1]) + data[mid_idx]) / 2.0;
        }

        // Mode
        int mode = data[0];
        int max_freq = 0;
        int current_freq = 0;
        int current_val = data[0];

        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i] == current_val) {
                current_freq++;
            }
            else {
                if (current_freq > max_freq) {
                    max_freq = current_freq;
                    mode = current_val;
                }
                current_val = data[i];
                current_freq = 1;
            }
        }
        if (current_freq > max_freq) { // Check for the last element's frequency
            mode = current_val;
        }

        std::ostringstream oss;
        oss << "Statistics Analysis:\n"
            << "  Min: " << min_val << "\n"
            << "  Max: " << max_val << "\n"
            << "  Mean: " << mean << "\n"
            << "  Median: " << median << "\n"
            << "  Mode: " << mode;
        return oss.str();
    }
};

// Example Usage (for demonstration and testing)
int main() {
    std::vector<int> sample_data = { 5, 2, 8, 1, 9, 4, 7, 3, 9, 2, 8, 5, 1, 9, 7, 3, 6, 4, 0, 10 };

    // Test SearchAnalyzer
    SearchAnalyzer search_analyzer(sample_data);
    std::cout << search_analyzer.analyze() << std::endl;

    // Test StatisticsAnalyzer
    StatisticsAnalyzer stats_analyzer(sample_data);
    std::cout << stats_analyzer.analyze() << std::endl;

    return 0;
}