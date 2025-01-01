#include <GL/glut.h>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>

void updateGame(int value);


// Pipe structure
struct Pipe {
    float x;
    float gapY;
    const float width = 0.2f;
    const float gapHeight = 0.6f;

    Pipe(float x, float gapY) : x(x), gapY(gapY) {}
};

std::vector<Pipe> pipes;
float birdY = 0.0f;
float birdVelocity = 0.0f;
float gravity = -0.001f;
float jumpStrength = 0.02f;
bool gameOver = false;
int score = 0;

void displayScore() {
    std::ostringstream scoreStream;
    scoreStream << "Score: " << score;
    std::string scoreText = scoreStream.str();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.9f, 0.9f);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void displayGameOverScreen() {
    std::string gameOverText = "Game Over! Press 'R' to restart";

    glColor3f(1.0f, 0.0f, 0.0f); // Red color for Game Over text
    glRasterPos2f(-0.5f, 0.0f);
    for (char c : gameOverText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void resetGame() {
    birdY = 0.0f;               // Reset bird's position
    birdVelocity = 0.0f;        // Reset bird's velocity
    score = 0;                  // Reset score
    gameOver = false;           // Reset game over state

    // Reinitialize pipes
    pipes.clear();
    pipes.push_back(Pipe(0.5f, 0.0f));
    pipes.push_back(Pipe(1.2f, 0.5f));

    // Restart game loop
    glutTimerFunc(16, updateGame, 0);
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameOver) {
        displayGameOverScreen();
    }
    else {
        // Draw background
        glColor3f(0.0f, 0.5f, 1.0f); // Sky blue
        glBegin(GL_QUADS);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
        glEnd();

        // Draw pipes
        for (const Pipe& pipe : pipes) {
            // Top pipe
            glColor3f(0.0f, 1.0f, 0.0f); // Green
            glBegin(GL_QUADS);
            glVertex2f(pipe.x, pipe.gapY + pipe.gapHeight);
            glVertex2f(pipe.x + pipe.width, pipe.gapY + pipe.gapHeight);
            glVertex2f(pipe.x + pipe.width, 1.0f);
            glVertex2f(pipe.x, 1.0f);
            glEnd();

            // Bottom pipe
            glColor3f(0.0f, 1.0f, 0.0f); // Green
            glBegin(GL_QUADS);
            glVertex2f(pipe.x, -1.0f);
            glVertex2f(pipe.x + pipe.width, -1.0f);
            glVertex2f(pipe.x + pipe.width, pipe.gapY);
            glVertex2f(pipe.x, pipe.gapY);
            glEnd();
        }

        // Draw bird
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow
        glBegin(GL_QUADS);
        glVertex2f(-0.05f, birdY - 0.05f);
        glVertex2f(0.05f, birdY - 0.05f);
        glVertex2f(0.05f, birdY + 0.05f);
        glVertex2f(-0.05f, birdY + 0.05f);
        glEnd();

        // Display score
        displayScore();
    }

    glutSwapBuffers();
}

void updateGame(int value) {
    if (gameOver) return;

    birdVelocity += gravity;
    birdY += birdVelocity;

    // Update pipes
    for (Pipe& pipe : pipes) {
        pipe.x -= 0.01f;
        if (pipe.x < -1.0f) {
            pipe.x = 1.0f;
            pipe.gapY = ((rand() % 100) / 100.0f) * 1.5f - 0.75f;
            score++;
        }

        // Check for collisions
        if (pipe.x < -0.05f && pipe.x + pipe.width > -0.05f) {
            if (birdY < pipe.gapY || birdY > pipe.gapY + pipe.gapHeight) {
                gameOver = true;
            }
        }
    }

    // Check if the bird hits the bottom boundary
    if (birdY < -1.0f) {
        gameOver = true;
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateGame, 0);
}

void handleKeypress(unsigned char key, int x, int y) {
    if (key == ' ') {
        if (!gameOver) {
            birdVelocity = jumpStrength; // Bird jumps on spacebar
        }
    }
    else if (key == 'r') {
        resetGame(); // Restart the game
        glutPostRedisplay(); // Refresh display
    }
}

void initGame() {
    srand(static_cast<unsigned int>(time(0)));

    pipes.push_back(Pipe(0.5f, 0.0f));
    pipes.push_back(Pipe(1.2f, 0.5f));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Flappy Bird");

    initGame();

    glutDisplayFunc(renderScene);
    glutKeyboardFunc(handleKeypress);
    glutTimerFunc(16, updateGame, 0);

    glutMainLoop();
    return 0;
}
