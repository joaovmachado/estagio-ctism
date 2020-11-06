void listDir(const char * dirname) {
  Serial.printf("Listing directory: %s\n", dirname);

  Dir root = LittleFS.openDir(dirname);

  while (root.next()) {
    File file = root.openFile("r");
    Serial.print("  FILE: ");
    Serial.print(root.fileName());
    Serial.print("  SIZE: ");
    Serial.println(file.size());
    file.close();
  }
}

String readFile(const char * path) {
  Serial.printf("Lendo %s", path);

  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.println(" [ERRO]");
    Serial.println("Houve uma falha na tentativa de ler o arquivo");
    return "15000";
  }
  
  String output;
  while (file.available()) {
    output = file.readStringUntil('\n');
  }
  Serial.println(" [OK]");
  Serial.println("   ..." + output);
  file.close();
  return output;
}

void writeFile(const char * path, String output) {
  Serial.printf("Escrevendo %s em %s...", output.c_str(), path);

  File file = LittleFS.open(path, "w");
  if (!file) {
    Serial.print(" [ERRO]");
    Serial.println("Houve uma falha na tentativa de abrir o arquivo para escrita");
    return;
  }
  if (file.print(output)) {
    Serial.println(" [OK]");
  } else {
    Serial.println(" [ERRO]");
  }
  
  file.close();
}

void appendFile(const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = LittleFS.open(path, "a");
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (LittleFS.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (LittleFS.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}
