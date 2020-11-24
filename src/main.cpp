#include "utils.h"

// GLobal variables
std::vector<float> mapContour1Points;
std::vector<float> mapContour2Points; //contains anchor points and handle points for cubic bezier
std::vector<float> Contour1pts;
std::vector<float> Contour2pts;


int width = 640, height = 640; 
bool controlPointsUpdated = false;
extern bool meshCreated;


int main(int, char* argv[])
{
    GLFWwindow* window = setupWindow(width, height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
	glUseProgram(shaderProgram);

    float xcoord, ycoord; 
    ifstream inFile1, inFile2,inFile3, inFile4;

    inFile1.open("./images/mancoordX.txt");
    inFile2.open("./images/mancoordY.txt");
    if (!inFile1 || !inFile2) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    int j =0;
    while (inFile1 >> xcoord && inFile2>> ycoord) {
        addPoints(Contour1pts, xcoord, ycoord, width, height, -1);
    }
         
    inFile1.close();
    inFile2.close();

    inFile3.open("./images/dancingCoordX.txt");
    inFile4.open("./images/dancingCoordY.txt");
    if (!inFile3 || !inFile4) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    while (inFile3 >> xcoord && inFile4>> ycoord) {
        addPoints(Contour2pts, xcoord, ycoord, width, height, -0.3);
    }
    inFile3.close();
    inFile4.close();

    mapContour1Points.push_back(Contour1pts[0]);
    mapContour1Points.push_back(Contour1pts[1]);
    mapContour1Points.push_back(0.0);
    mapContour2Points.push_back(Contour2pts[0]);
    mapContour2Points.push_back(Contour2pts[1]);
    mapContour2Points.push_back(0.0);

    unsigned int  VBO_mapContour1Points, VBO_mapContour2Points, VBO_Contour1, VBO_Contour2;
    unsigned int  VAO_mapContour1Points, VAO_mapContour2Points, VAO_Contour1, VAO_Contour2;
    
    glGenBuffers(1, &VBO_mapContour1Points);
    glGenVertexArrays(1, &VAO_mapContour1Points);
    glGenBuffers(1, &VBO_mapContour2Points);
    glGenVertexArrays(1, &VAO_mapContour2Points);
    glGenBuffers(1, &VBO_Contour1);
    glGenVertexArrays(1, &VAO_Contour1);
    glGenBuffers(1, &VBO_Contour2); 
    glGenVertexArrays(1, &VAO_Contour2);
  

    //Display loop
    while (!glfwWindowShouldClose(window))
    {   
        
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Rendering
        showOptionsDialog(mapContour1Points, mapContour2Points, Contour1pts, Contour2pts ,io);

        ImGui::Render();

        // Add a new point on mouse click
        float x,y ;
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        
        if(io.MouseClicked[0] && !ImGui::IsAnyItemActive()){
            x = io.MousePos.x;
            y = io.MousePos.y;
            
            float rescaled_x = -1.0 + ((1.0*x - 0) / (width - 0)) * (1.0 - (-1.0));
            float rescaled_y = -1.0 + ((1.0*(height - y) - 0) / (height - 0)) * (1.0 - (-1.0));

            int s1 = Contour1pts.size();
            int s2 = Contour2pts.size();
            

            for(int i=0; i<(s1); i+=3) {
                float xA = rescaled_x - Contour1pts[i];
                float yA = rescaled_y - Contour1pts[i+1];
                float distSquare = xA*xA + yA*yA; //precision for checking point pick up
                
                if(distSquare<0.001){
                    mapContour1Points.push_back(Contour1pts[i]);
                    mapContour1Points.push_back(Contour1pts[i+1]);
                    mapContour1Points.push_back(0.0);
                    controlPointsUpdated = true;
                    std::cout<<"1"<<endl;
                    printPoints(mapContour1Points);
                    std::cout<<endl;
                    break;
                }
            }
            for(int i=0; i<(s2); i+=3) {
                float xA = rescaled_x - Contour2pts[i];
                float yA = rescaled_y - Contour2pts[i+1];
                float distSquare = xA*xA + yA*yA; //precision for checking point pick up
                
                if(distSquare<0.001){
                    mapContour2Points.push_back(Contour2pts[i]);
                    mapContour2Points.push_back(Contour2pts[i+1]);
                    mapContour2Points.push_back(0.0);
                    controlPointsUpdated = true;
                    std::cout<<"2"<<endl;
                    printPoints(mapContour2Points);
                    std::cout<<endl;
                    break;
                }
            }   
        }

        if(controlPointsUpdated) {
            
            glBindVertexArray(VAO_mapContour1Points);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_mapContour1Points);
            glBufferData(GL_ARRAY_BUFFER, mapContour1Points.size()*sizeof(GLfloat), &mapContour1Points[0], GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0); //Enable first attribute buffer (vertices)

            glBindVertexArray(VAO_mapContour2Points);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_mapContour2Points);
            glBufferData(GL_ARRAY_BUFFER, mapContour2Points.size()*sizeof(GLfloat), &mapContour2Points[0], GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0); //Enable first attribute buffer (vertices)

            controlPointsUpdated = false; // Finish all VAO/VBO updates before setting this to false.
        }

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO_mapContour1Points);
		glDrawArrays(GL_POINTS, 0, mapContour1Points.size()/3); // Draw points

        glBindVertexArray(VAO_mapContour2Points);
        glDrawArrays(GL_POINTS, 0, mapContour2Points.size()/3); // Draw points

        glBindVertexArray(VAO_Contour1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Contour1);
        glBufferData(GL_ARRAY_BUFFER, Contour1pts.size()*sizeof(GLfloat), &Contour1pts[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_STRIP, 0, Contour1pts.size()/3);


        glBindVertexArray(VAO_Contour2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_Contour2);
        glBufferData(GL_ARRAY_BUFFER, Contour2pts.size()*sizeof(GLfloat), &Contour2pts[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_STRIP, 0, Contour2pts.size()/3);

        glUseProgram(0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

    }

    // Delete VBO buffers
    
    glDeleteBuffers(1, &VBO_Contour1);
    glDeleteBuffers(1, &VBO_Contour2);
    glDeleteBuffers(1, &VBO_mapContour1Points);
    glDeleteBuffers(1, &VBO_mapContour2Points);
    //TODO:

    // Cleanup
    cleanup(window);
    return 0;
}
