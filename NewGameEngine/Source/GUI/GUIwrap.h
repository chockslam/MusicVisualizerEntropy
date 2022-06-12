#pragma once 

#include <string>
#include <memory>
class Texture;
class Graphics;
class GUIwrap {

public:
    static GUIwrap& getInstance()
    {
        static GUIwrap instance; // Guaranteed to be destroyed.
                              // Instantiated on first use.
        return instance;
    }
private:
    GUIwrap();
    ~GUIwrap();

    //};            // Constructor? (the {} brackets) are needed here.
    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
public:
    GUIwrap(GUIwrap const&) = delete;
    void operator=(GUIwrap const&) = delete;


    std::string getUpdatedWavFile();        // getter for filename
    void CreateTexture(Graphics& gfx);      // Initialize textures to be rendered in the GUI
    void DrawStatusBar(float params[3],   
        bool& playing,
        const int& viewIndicator,
        bool displayPlay = false,
        bool fileDialogActive = false,
        bool slidersActive = false,
        bool displayRatio = false,
        bool displayIndicators = false,
        bool displayCamPos = false,
        float x = 0.0f,
        float y = 0.0f,
        float z = 0.0f,
        float yaw = 0.0f,
        float pitch = 0.0f,
        float roll = 0.0f);
	void DrawFileDialog();                          // Draw file dialog (ImGuiFileDialog.h)
	void DrawSliders(float weightOfParams[3]);      // Draw interactive sliders that determine the weight of parameters
    bool isSlidersActive();
    bool isFileDialogActive();
    void setSlidersActive(bool status);
    void setFileDialogActive(bool status);
    void showFFT(double freq[], double magn[], float musParams[3]);     // Show fft graph (For development purposes)w
    void EnableDocking();
    void showDemoWindow();
    void CreateDockSpace(bool* p_open);

private:
    void makeStyle();                                                   // general make style 
    void FileDialogButton();
    void makeSliderButton();
    void makePauseButton(bool& playing);
    void DisplayCamPos(float x,float y, float z, float yaw, float pitch, float roll);
    void DisplayRation(float params[3]);                                 // Display ration of frequency domains (For development purposes)
    void ViewIndicator(const int &indicator);
    
    std::string filename;                                                // Filename string (chosen from file dialog)

    // Checks whether to draw sliders or file dialog windows.
    bool slidersActive;
    bool FileDialogActive;

    // Images to be rendered to Imgui interface
    std::shared_ptr<Texture> playTexture;
    std::shared_ptr<Texture> pauseTexture;
    std::shared_ptr<Texture> BassTexture;
    std::shared_ptr<Texture> MidTexture;
    std::shared_ptr<Texture> TrebleTexture;
    
};