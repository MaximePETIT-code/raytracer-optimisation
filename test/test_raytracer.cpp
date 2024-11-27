#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <limits.h>
#include <unistd.h>
#include <SceneLoader.hpp>
#include <Image.hpp>

// Function to check if a file exists
bool fileExists(const std::string &filename)
{
    std::ifstream file(filename);
    return file.good();
}

bool compareBinaryFiles(const std::string &expectedFilename, const std::string &generatedFilename)
{
    std::ifstream generatedFile(generatedFilename, std::ios::binary);
    std::ifstream expectedFile(expectedFilename, std::ios::binary);

    assert(generatedFile.is_open());
    assert(expectedFile.is_open());

    // Compare file sizes
    generatedFile.seekg(0, std::ios::end);
    expectedFile.seekg(0, std::ios::end);

    if (generatedFile.tellg() != expectedFile.tellg()) {
        return false; // Files are of different sizes
    }

    // Reset to beginning of files
    generatedFile.seekg(0, std::ios::beg);
    expectedFile.seekg(0, std::ios::beg);

    // Compare files byte by byte
    std::istreambuf_iterator<char> begin1(generatedFile);
    std::istreambuf_iterator<char> begin2(expectedFile);
    std::istreambuf_iterator<char> end;

    return std::equal(begin1, end, begin2);
}

int renderTest(const std::string& sceneFilePath, const std::string& referenceFilePath, const std::string& outputFilePath)
{
    std::cout << "Running test for scene: " << sceneFilePath << std::endl;

    // Load the scene
    auto [scene, camera, image] = SceneLoader::Load(sceneFilePath);

    // Render the scene
    camera->render(*image, *scene);

    // Save the output image
        std::string dynamicImagePath = outputFilePath;

    image->writeFile(dynamicImagePath);

    // Compare the output image with the expected output image
    bool result = compareBinaryFiles(referenceFilePath, outputFilePath);
    assert(result);

    std::cout << "Test passed for scene: " << sceneFilePath << std::endl;

    delete scene;
    delete camera;
    delete image;

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <scene_file> <reference_file> <output_file>" << std::endl;
        return 1;
    }

    std::string sceneFilePath = argv[1];
    std::string referenceFilePath = argv[2];
    std::string outputFilePath = argv[3];

    return renderTest(sceneFilePath, referenceFilePath, outputFilePath);
}