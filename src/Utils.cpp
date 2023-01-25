#include "Utils.h"
#include "Core/Debug.h"


glm::mat4 modelPivotSquare(glm::mat4& model, float rotation, float w, float h, uint pivot) {
    switch (pivot) {
        case 0: {
            model = glm::translate(model, glm::vec3(0.5 * w, 0.5 * h, 0.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); 
            model = glm::translate(model, glm::vec3(-0.5 * w, -0.5 * h, 0.0f));
            return model;
            break;
        }
        case 1: {
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); 
            return model;
        }
        case 2: {
            model = glm::translate(model, glm::vec3(w, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); 
            model = glm::translate(model, glm::vec3(-w, 0.0f, 0.0f));
            return model;
            break;
        }
        case 3: {
            model = glm::translate(model, glm::vec3(0.0f, h, 0.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); 
            model = glm::translate(model, glm::vec3(0.0f, -h, 0.0f));
            return model;
            break;
        }
        case 4: {
            model = glm::translate(model, glm::vec3(w, h, 0.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); 
            model = glm::translate(model, glm::vec3(-w, -h, 0.0f));
            return model;
            break;
        }
        default: {
            log("Invalid pivot number", ERROR);
            throw;
        }
    }
}

int randNum(int min, int max) {
    return (rand() % (max - min + 1) + min);
}
