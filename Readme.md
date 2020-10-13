![](logo.svg)

## Radiate -- a modern wifi captive portal

Radiate is built to solve one problem: connect your wifi device to wifi.

Files are stored and served from SPIFFS -- no embedding html in your c++!

## Installation

### PlatformIO

```console
$ pio lib install https://github.com/tumblerlock/radiate.git#master
```

### Additional Files

Radiate depends on HTML which is uploaded to the ESP filesystem. From your project directory run:

```console
$ bash -c "$(curl -s https://raw.githubusercontent.com/tumblerlock/radiate/master/setup.sh)"
```

Or follow the simple steps outlined in setup.sh to manually install radiate and helper utilities into your project.

## Usage Example

```cpp
#include <WiFiClient.h>
#include <radiate.h>

Radiate *radiate = new Radiate("My IoT Project");

void setup() {
  // if you need your device to forget it's saved wifi configuration:
  // radiate->obliviate();
  
  // Attempt to connect to stored wifi, or create an AP for provisioning:
  radiate->connectOrPortalize();
}

void loop() {
  // fetch the local ip address from the WiFi library:
  Serial.print(WiFi.localIP());
  delay(1000);
}
```

## Developing the portal html (required)

### Prerequisites

- [minify](https://github.com/tdewolff/minify) - `brew install tdewolff/tap/minify`
- Some static html server. The bundled script uses ruby+webrick, and any system ruby from the last 10 years should do. Alternatively, [there are multitude of static file server one-liners](https://gist.github.com/willurd/5720255) and any of them should do.

### Workflow

If you're just using the stock landing page, you can ignore this. Skip to **Upload the web assets to SPIFFS**.

To save write cycles on SPIFFS, develop the portal locally. 

 - `scripts/serve_www` will start a webserver running at [http://localhost:3000](http://localhost:3000).
 - The server is configured to serve static assets from the www directory. 
 - Visit [/portal.html](http://localhost:3000/portal.html) and edit/reload as you go.

Once the portal is ready for a deeper integration test, compress and upload the assets in the next section.

Add a `radiate->obliviate();` at some easy to access logic path in order to facilitate testing.

### Upload the web assets to SPIFFS

Two helper scripts are provided to ease the process of installing and updating the web assets to SPIFFS.

- `scripts/compress_www` will minify and gzip web assets, placing the output in the [`data` directory](https://docs.platformio.org/en/latest/platforms/espressif32.html#uploading-files-to-file-system-spiffs).
- `scripts/upload_www` instructs platformio to upload the filesystem (`platformio run --target uploadfs`).
