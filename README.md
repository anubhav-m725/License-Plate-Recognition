# License Plate Detection and OCR (C++ & OpenCV)

This project detects license plates in images using OpenCV and extracts their text using Tesseract OCR. It's built in C++ and designed as a minor college project with a simple but functional approach.

## Features

- Detects license plates using Haar Cascade classifiers  
- Automatically crops the plate region from an image  
- Extracts text from the plate using Tesseract OCR  
- Corrects common OCR mistakes (like replacing `0` with `O` or `A` with `4` appropriately)  
- Stores the cropped plate and result in a timestamped folder inside `data/`

## Sample Workflow

1. Place images inside the `images/` directory  
2. Run the program, choose the image number  
3. Plate is detected, text extracted using OCR  
4. Cropped image and output are saved in `data/<timestamp>/`

## Tech Stack

- C++  
- OpenCV (via vcpkg)  
- Tesseract OCR  
- MSVC (Microsoft C++ compiler)  
- VS Code

## How to Run

1. Clone the repo  
2. Make sure you have:
   - OpenCV installed via vcpkg
   - Tesseract installed and added to PATH  
3. Place images inside the `images/` folder  
4. Open terminal in the project directory  
5. Compile using your build script or run the existing `.exe`  
6. Follow the prompts

## Notes

- Only the first detected license plate in an image is used  
- Correction is based on assumed Indian plate format: **AA00AA0000**
- You can generate more of such plates [here](https://www.mrplates.com.au/designer)  
- This is a basic prototype and may not work on all types of plates  

## Future Improvements

- Use deep learning-based plate detection (YOLO, etc.)  
- Support video and camera input  
- Handle multiple plates in a single image  
- Build a GUI or web interface  

## Author

Made with guidance and curiosity as a first-year student project.  
Feel free to fork, suggest improvements, or just say hi!
