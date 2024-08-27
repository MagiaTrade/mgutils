//
// Created by Arthur Motelevicz on 26/08/24.
//
#include <catch2/catch.hpp>
#include <mgutils/Files.h>
#include <filesystem>
#include <string>

using namespace mgutils;

TEST_CASE("Files utility class tests", "[files]") {
  const std::string resourceDir = "resources/";
  const std::string testFile = resourceDir + "test_file.txt";
  const std::string testDir = resourceDir + "test_dir";
  const std::string nestedDir = resourceDir + "sub_dir";
  const std::string nestedFile = nestedDir + "/nested_file.txt";
  const std::string newFile = resourceDir + "new_file.txt";
  const std::string newDir = resourceDir + "new_dir";
  const std::string newNestedDir = resourceDir + "new_nested_dir/sub_dir";

  SECTION("Check file and directory status")
  {
    REQUIRE(Files::isFile(testFile));
    REQUIRE_FALSE(Files::isFile(testDir));
    REQUIRE(Files::isDirectory(testDir));
    REQUIRE_FALSE(Files::isDirectory(testFile));
  }

  SECTION("Check absolute and relative path")
  {
    REQUIRE(Files::isAbsolutePath("/absolute/path"));
    REQUIRE_FALSE(Files::isAbsolutePath("relative/path"));
    REQUIRE(Files::isRelativePath("relative/path"));
    REQUIRE_FALSE(Files::isRelativePath("/absolute/path"));
  }

  SECTION("Read and write file content")
  {
    std::string content;
    REQUIRE_NOTHROW(content = Files::readFile(testFile));
    REQUIRE(content == "Hello, World!\n");

    std::string newContent = "New content!";
    REQUIRE_NOTHROW(Files::writeFile(newFile, newContent));
    REQUIRE(Files::isFile(newFile));

    std::string readContent;
    REQUIRE_NOTHROW(readContent = Files::readFile(newFile));
    REQUIRE(readContent == newContent);

    // Cleanup
    std::filesystem::remove(newFile);
  }

  SECTION("List files and directories") {
    auto files = Files::listFiles(resourceDir);
    auto directories = Files::listDirectories(resourceDir);

    REQUIRE_FALSE(files.empty());
    REQUIRE_FALSE(directories.empty());
    REQUIRE(std::find(files.begin(), files.end(), testFile) != files.end());
    REQUIRE(std::find(directories.begin(), directories.end(), testDir) != directories.end());
  }

  SECTION("Append and remove path components") {
    std::string appendedPath = Files::appendPath(testDir, "appended");
    REQUIRE(appendedPath == testDir + "/appended");

    std::string removedComponent = Files::removeLastComponent(appendedPath);
    REQUIRE(removedComponent == testDir);
  }

  SECTION("Create directories and files") {
    REQUIRE_NOTHROW(Files::createDirectory(newDir));
    REQUIRE(Files::isDirectory(newDir));

    REQUIRE_NOTHROW(Files::createDirectory(newNestedDir, true));
    REQUIRE(Files::isDirectory(newNestedDir));

    REQUIRE_NOTHROW(Files::createFile(newDir + "/new_file.txt"));
    REQUIRE(Files::isFile(newDir + "/new_file.txt"));

    // Cleanup
    std::filesystem::remove_all(newDir);
    std::filesystem::remove_all("resources/new_nested_dir");
  }
}
