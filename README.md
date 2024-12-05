# Perlin Noise Project - README

## Overview

This project is a custom implementation of 2D Perlin Noise in Unreal Engine 5, designed for procedural texture generation. It includes a `UPerlinNoiseLibrary` class for Perlin noise calculation and an `ANoiseTester` actor to generate and save Perlin noise textures as assets in the Unreal Editor.

## Features

- **Custom Perlin Noise Algorithm**: Generate 2D Perlin noise with configurable scale, seed, persistence, and octaves.
- **Dynamic Texture Generation**: Generate textures based on noise values and save them as assets.
- **Integration with Unreal Editor**: Automatically updates the Content Browser with the generated texture.

## Setup and Usage

### Setup

1. Clone or download this project.
2. Copy the source files into your Unreal Engine project's `Source` directory:
   - **`PerlinNoiseLibrary.h` and `PerlinNoiseLibrary.cpp`**: Located in the `Source/[YourProjectName]` directory.
   - **`NoiseTester.h` and `NoiseTester.cpp`**: Same directory as above.

4. Compile the project using **Visual Studio** or **Rider**.

### Usage

1. **Create and Add the Noise Tester Actor**:
   - Create a new Blueprint from the C++ class `ANoiseTester.
   - Drag and drop the `ANoiseTester` actor into your level.

2. **Configure Parameters**:
   - Select the `ANoiseTester` actor in the level.
   - Configure the following properties in the Details panel:
     - **`Size`**: The resolution of the generated texture (e.g., 512 for 512x512).
     - **`AssetPath`**: Path to save the generated texture (e.g., `/Game/MyTexture`).
     - **`Scale`**: Scale factor for the Perlin noise.
     - **`Seed`**: Seed value for deterministic noise generation.
     - **`Persistence`**: Controls the amplitude of noise layers.
     - **`Octaves`**: Number of layers to combine in fractal Brownian motion.

3. **Generate the Texture**:
   - Play the level, the texture will be generated and saved to the specified asset path.

4. **View the Texture**:
   - Open the Content Browser in Unreal Editor. The texture will appear at the specified path.

### Notes

- Ensure the `AssetPath` is valid and within the `/Game/` directory.
- The `Size` property can significantly impact performance for high-resolution textures.

## Debugging and Logging

- Logs are output to the Unreal Engine log window.
- Use `UE_LOG` statements in the source code to trace steps during texture generation or debug any issues.

---

Feel free to customize the parameters and extend the functionality to suit your procedural generation needs!