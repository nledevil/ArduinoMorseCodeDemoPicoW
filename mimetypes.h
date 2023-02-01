// Determine Mime/Content Type given the file extension, this should be most of the ones we would use
String getContentType(String filename) {
  if (filename.endsWith(".avi")) return "video/x-msvideo";
  else if (filename.endsWith(".bmp")) return "image/bmp";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".csv")) return "text/csv";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".jpeg")) return "image/jpeg";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".mp3")) return "audio/mpeg";
  else if (filename.endsWith(".mp4")) return "video/mp4";
  else if (filename.endsWith(".mpeg")) return "audio/mpeg";
  else if (filename.endsWith(".otf")) return "font/otf";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".pdf")) return "application/pdf";
  else if (filename.endsWith(".svg")) return "image/svg+xml";
  else if (filename.endsWith(".tif")) return "image/tiff";
  else if (filename.endsWith(".tiff")) return "image/tiff";
  else if (filename.endsWith(".ttf")) return "font/ttf";
  else if (filename.endsWith(".wav")) return "audio/wav";
  else if (filename.endsWith(".woff")) return "font/woff";
  else if (filename.endsWith(".woff2")) return "font/woff2";
  else if (filename.endsWith(".xml")) return "application/xml";
  else if (filename.endsWith(".zip")) return "application/zip";
  else if (filename.endsWith(".7z")) return "application/x-7z-compressed";
  return "text/plain";
}