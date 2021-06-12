String
readFile(const char * path) {
  Serial.printf("Lendo %s\n", path);

  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.println(" [FAILED]");
    Serial.println("Houve uma falha na tentativa de ler o arquivo");
    return "failed";
  }
  
  String output;
  while (file.available()) {
    output = file.readStringUntil('\n');
    Serial.println(output);
  }
  Serial.println(" [OK]");

  file.close();
  return output;
}


void
writeFile(const char * path, String input) {
  Serial.printf("\nEscrevendo %s em %s...", input.c_str(), path);

  File file = LittleFS.open(path, "w"); //abre o caminho do arquivo no modo escrita
  if (!file) {
    error_status = 3;
    no_error = false;
    Serial.print(" [FAILED]");
    Serial.println("Houve uma falha na tentativa de abrir o arquivo para escrita");
    return;
  }
  if (file.print(input)) {
    Serial.println(" [OK]");
  } else {
    error_status = 3;
    no_error = false;
    Serial.println(" [FAILED]");
  }
  
  file.close();
}


void
appendFile(const char * path, String message) {
  Serial.printf("Anexando ao arquivo: %s", path);

  File file = LittleFS.open(path, "a");
  if (!file) {
    error_status = 3;
    no_error = false;
    Serial.println(" [FAILED]");
    Serial.println("   Não foi possível abrir o arquivo");
    return;
  }
  if (file.print(message)) {
    Serial.println(" [OK]");
  } else {
    error_status = 3;
    no_error = false;
    Serial.println(" [FAILED]");
    Serial.println("   Não foi possível anexar a mesagem ao arquivo");
  }
  file.close();
}


void saveAPIP() {
  String ip = WiFi.localIP().toString();
  Serial.printf("Salvando IP %s", ip.c_str());
  if (ip != "(IP unset)")
    writeFile("/data/ipaddress.txt", ip);
  else {
    error_status = 3;
    no_error = false;
    Serial.println(" [Failed]");
    return;
  }
  Serial.println(" [OK]");
}


void renameFile(const char * originalPath, const char * renamedPath) {
  Serial.printf("Renomeando o arquivo %s para %s", originalPath, renamedPath);
  if (LittleFS.rename(originalPath, renamedPath)) {
    Serial.println(" [OK]");
  } else {
    error_status = 3;
    no_error = false;
    Serial.println(" [FAILED]");
  }
}

void deleteFile(const char * path) {
  Serial.printf("Removendo %s\n", path);
  if (LittleFS.remove(path)) {
    Serial.println(" [OK]");
  } else {
    error_status = 3;
    no_error = false;
    Serial.println(" [FAILED]");
  }
}

void listDir(const char * dirname) {
  Serial.printf("Listando o diretório: %s\n", dirname);

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
