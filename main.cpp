#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <filesystem>
#include <cctype>

namespace fs = std::filesystem;

// Function to correct plate text
std::string correctPlate(const std::string &raw)
{
    std::string corrected = raw;

    for (int i = 0; i < corrected.size(); ++i)
    {
        char &c = corrected[i];

        if ((i == 0 || i == 1 || i == 4 || i == 5) && isdigit(c))
        {
            if (c == '0')
                c = 'O';
            else if (c == '1')
                c = 'I';
            else if (c == '4')
                c = 'A';
            else if (c == '8')
                c = 'B';
        }

        else if ((i == 2 || i == 3 || i >= 6) && isalpha(c))
        {
            if (c == 'O')
                c = '0';
            else if (c == 'I')
                c = '1';
            else if (c == 'Z')
                c = '2';
            else if (c == 'A')
                c = '4';
            else if (c == 'B')
                c = '8';
        }
    }

    return corrected;
}

int main()
{
    std::vector<std::string> availableImages;
    std::cout << "Available images:\n";
    for (const auto &entry : fs::directory_iterator("images"))
    {
        std::string name = entry.path().filename().string();
        std::cout << availableImages.size() + 1 << ". " << name << "\n";
        availableImages.push_back(name);
    }

    if (availableImages.empty())
    {
        std::cerr << "No images found in 'images/' directory.\n";
        return -1;
    }

    // Choose image
    int choice;
    std::cout << "Choose image number: ";
    std::cin >> choice;
    std::cin.ignore();

    if (choice < 1 || choice > availableImages.size())
    {
        std::cerr << "Invalid choice.\n";
        return -1;
    }

    std::string imagePath = "images/" + availableImages[choice - 1];
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        std::cerr << "Failed to read image: " << imagePath << "\n";
        return -1;
    }

    // Preprocess
    cv::Mat gray, blurred;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);
    cv::equalizeHist(blurred, gray);

    // Load Haar cascade
    cv::CascadeClassifier plateCascade;
    if (!plateCascade.load("haarcascade_russian_plate_number.xml"))
    {
        std::cerr << "Error loading Haar Cascade!\n";
        return -1;
    }

    std::vector<cv::Rect> plates;
    plateCascade.detectMultiScale(gray, plates, 1.1, 10, 0, cv::Size(50, 50));

    if (plates.empty())
    {
        std::cout << "No plates detected.\n";
        return 0;
    }

    // Take plate
    cv::Rect plateRegion = plates[0];
    cv::Mat croppedPlate = image(plateRegion);

    // Preprocess cropped plate
    cv::cvtColor(croppedPlate, croppedPlate, cv::COLOR_BGR2GRAY);
    cv::threshold(croppedPlate, croppedPlate, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // Create data folder
    if (!fs::exists("data"))
    {
        fs::create_directory("data");
    }

    // Generate timestamp
    std::string timestamp = std::to_string(time(0));
    std::string folderName = "data/" + timestamp;
    fs::create_directory(folderName);

    // Put cropped img and txt file in timestamp folder
    std::string croppedName = folderName + "/cropped_" + timestamp + ".png";
    cv::imwrite(croppedName, croppedPlate);
    std::cout << "Cropped plate saved as: " << croppedName << "\n";

    // Run tesseract
    std::string txtOutputName = folderName + "/output_" + timestamp;
    std::string command = "tesseract " + croppedName + " " + txtOutputName + " --psm 7 -c tessedit_char_whitelist=ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    system(command.c_str());

    // Read OCR result
    std::ifstream ocrResult(txtOutputName + ".txt");
    std::string plateText((std::istreambuf_iterator<char>(ocrResult)), std::istreambuf_iterator<char>());

    // Correct the OCR result using correctPlate()
    std::string correctedPlateText = correctPlate(plateText);

    // Display plate
    std::cout << "Detected Plate Text: " << correctedPlateText << "\n";

    // Overwrite txt file
    std::ofstream correctedOutput(txtOutputName + ".txt");
    correctedOutput << correctedPlateText;
    correctedOutput.close();
    return 0;
}
