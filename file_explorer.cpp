#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
using namespace std;

// Function to display directory contents
void showDirectory(const char* path) {
    DIR* dirPtr = opendir(path);
    if (!dirPtr) {
        cout << "Unable to open directory: " << path << endl;
        return;
    }
    cout << "Contents of directory: " << path << endl;
    struct dirent* item;
    while ((item = readdir(dirPtr)) != nullptr) {
        cout << item->d_name << endl;
    }
    closedir(dirPtr);
}

// Function to change directory
bool switchDirectory(const char* newPath) {
    return (chdir(newPath) == 0);
}

// Function to create a new file
void makeFile(const char* fileName) {
    FILE* file = fopen(fileName, "w");
    if (file) {
        fputs("New file created", file);
        fclose(file);
        cout << "File created successfully: " << fileName << endl;
    } else {
        cout << "Error: Could not create " << fileName << endl;
    }
}

// Function to remove a file
void removeFile(const char* fileName) {
    if (remove(fileName) == 0)
        cout << "File deleted: " << fileName << endl;
    else
        cout << "Failed to delete: " << fileName << endl;
}

// Function to rename/move a file
void renameFile(const char* src, const char* dest) {
    if (rename(src, dest) == 0)
        cout << "File moved/renamed from " << src << " to " << dest << endl;
    else
        cout << "Error: Could not move/rename file." << endl;
}

// Function to copy a file
void duplicateFile(const char* src, const char* dest) {
    FILE* inFile = fopen(src, "r");
    if (!inFile) { cout << "Cannot open source file.\n"; return; }

    FILE* outFile = fopen(dest, "w");
    if (!outFile) { 
        cout << "Cannot create destination file.\n";
        fclose(inFile);
        return; 
    }

    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), inFile)) > 0)
        fwrite(buf, 1, n, outFile);

    fclose(inFile);
    fclose(outFile);
    cout << "Copied file from " << src << " to " << dest << endl;
}

// Function to search for a file (non-recursive)
void findFile(const char* path, const char* name) {
    DIR* dir = opendir(path);
    if (!dir) {
        cout << "Unable to open directory for search.\n";
        return;
    }
    bool located = false;
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strstr(entry->d_name, name)) {
            cout << "Found: " << entry->d_name << endl;
            located = true;
        }
    }
    if (!located)
        cout << "No file found with name containing \"" << name << "\" in " << path << endl;
    closedir(dir);
}

// Function to display file permissions
void displayPermissions(const char* fileName) {
    struct stat fileInfo{};
    if (stat(fileName, &fileInfo) == 0) {
        cout << "Permissions for " << fileName << ": ";
        cout << ((fileInfo.st_mode & S_IRUSR) ? "r" : "-")
             << ((fileInfo.st_mode & S_IWUSR) ? "w" : "-")
             << ((fileInfo.st_mode & S_IXUSR) ? "x" : "-") << endl;
    } else {
        cout << "Error: File not found.\n";
    }
}

// Function to make a file read-only
void makeReadOnly(const char* fileName) {
    if (chmod(fileName, S_IRUSR | S_IRGRP | S_IROTH) == 0)
        cout << "File set to read-only: " << fileName << endl;
    else
        cout << "Failed to change permissions.\n";
}

int main() {
    char currentDir[1024];
    getcwd(currentDir, sizeof(currentDir));
    cout << "Starting in: " << currentDir << endl;

    int choice;
    do {
        cout << "\n====== File Explorer Menu ======\n";
        cout << "1. List Directory\n";
        cout << "2. Change Directory\n";
        cout << "3. Create File\n";
        cout << "4. Delete File\n";
        cout << "5. Move/Rename File\n";
        cout << "6. Copy File\n";
        cout << "7. Search File\n";
        cout << "8. Show Permissions\n";
        cout << "9. Set Read-Only Permissions\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                getcwd(currentDir, sizeof(currentDir));
                showDirectory(currentDir);
                break;
            case 2: {
                char path[1024];
                cout << "Enter directory path: ";
                cin.getline(path, sizeof(path));
                if (switchDirectory(path))
                    cout << "Directory changed successfully.\n";
                else
                    cout << "Failed to change directory.\n";
                break;
            }
            case 3: {
                char file[1024];
                cout << "Enter new file name: ";
                cin.getline(file, sizeof(file));
                makeFile(file);
                break;
            }
            case 4: {
                char file[1024];
                cout << "Enter file name to delete: ";
                cin.getline(file, sizeof(file));
                removeFile(file);
                break;
            }
            case 5: {
                char src[1024], dest[1024];
                cout << "Enter source file: ";
                cin.getline(src, sizeof(src));
                cout << "Enter destination file: ";
                cin.getline(dest, sizeof(dest));
                renameFile(src, dest);
                break;
            }
            case 6: {
                char src[1024], dest[1024];
                cout << "Enter source file: ";
                cin.getline(src, sizeof(src));
                cout << "Enter destination file: ";
                cin.getline(dest, sizeof(dest));
                duplicateFile(src, dest);
                break;
            }
            case 7: {
                char fileName[1024];
                cout << "Enter file name to search: ";
                cin.getline(fileName, sizeof(fileName));
                getcwd(currentDir, sizeof(currentDir));
                findFile(currentDir, fileName);
                break;
            }
            case 8: {
                char fileName[1024];
                cout << "Enter file name to show permissions: ";
                cin.getline(fileName, sizeof(fileName));
                displayPermissions(fileName);
                break;
            }
            case 9: {
                char fileName[1024];
                cout << "Enter file name to make read-only: ";
                cin.getline(fileName, sizeof(fileName));
                makeReadOnly(fileName);
                break;
            }
            case 0:
                cout << "Exiting File Explorer.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
