#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Softmax function that takes action scores and returns the probability distribution
void Softmax(float scores[], float probabilities[], int length) {
    float maxScore = scores[0];
    for (int i = 1; i < length; i++) {
        if (scores[i] > maxScore) {
            maxScore = scores[i]; // Find max score to prevent potential overflow
        }
    }

    float sum = 0.0f;
    for (int i = 0; i < length; i++) {
        probabilities[i] = exp(scores[i] - maxScore); // Subtract maxScore for numerical stability
        sum += probabilities[i];
    }

    for (int i = 0; i < length; i++) {
        probabilities[i] /= sum; // Normalize to get probabilities
    }
}

// Function to sample an action based on softmax probabilities
int Sample(float probabilities[]) {
    float r = (float)rand() / (float)RAND_MAX;
    float cumProb = 0.0f;
    for (int i = 0; i < 3; i++) {
        cumProb += probabilities[i];
        if (r < cumProb) {
            return i;
        }
    }
    return 2; // Return the last action if none other was selected
}

int main() {
    srand((unsigned int)time(NULL));

    const int numGames = 1000; 
    const int outcomes[9] = {
        0, -1,  1,
        1,  0, -1,
        -1,  1,  0
    };
    
    float probabilities[3] = { 0.0f, 0.0f, 0.0f };
    float scores[3] = { 1.0f, 0.0f, -1.0f }; // Use to accumulate scores for softmax

    float opp1Probabilities[3] = { 0.3f, 0.3f, 0.4f }; // Opponent's probabilities
    float opp2Probabilities[3] = { 0.3f, 0.4f, 0.3f }; // Opponent's probabilities

    for (int iteration = 2000; iteration--;)
    {
        int actionCount[3] = { 0, 0, 0 };

        Softmax(scores, probabilities, 3);
        for (int i = numGames; i--;)
        {
            int action1 = Sample(probabilities);
            int action2;
            if (iteration % 2)
                action2 = Sample(opp1Probabilities);
            else
                action2 = Sample(opp2Probabilities);
            int outcome = outcomes[action1 * 3 + action2];
            actionCount[action1] += outcome;
        }

        // Adjust scores based on the outcomes
        float learningRate = 0.1f / (float)(numGames);
        for (int i = 0; i < 3; i++) {
            scores[i] += learningRate * actionCount[i];
        }

        // Periodically print the probabilities
        if (iteration % 100 == 0) {
            printf("Rock: %.2f%%, Paper: %.2f%%, Scissors: %.2f%%\n",
                   probabilities[0] * 100,
                   probabilities[1] * 100,
                   probabilities[2] * 100);
            // printf("Rock: %.2f%%, Paper: %.2f%%, Scissors: %.2f%%\n\n",
            //     actionCount[0] / (float)numGames,
            //     actionCount[1] / (float)numGames,
            //     actionCount[2] / (float)numGames);
        }
    }
    
    return 0;
}
