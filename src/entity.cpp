#include "entity.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

entity::entity() {
    this->vertCount = verts.size();

}

entity::entity(std::vector<float> verts) {
    this->verts = verts;
    this->vertCount = verts.size();
}

entity::entity(std::vector<float> verts, std::vector<unsigned int> indicies) {
    this->verts = verts;
    this->indicies = indicies;
    this->vertCount = verts.size();
    this->indexCount = indicies.size();
    
}

entity::entity(entity &copy) {
    this->vertCount = copy.getVertCount();
    //glm::vec3 newverts[vertCount];
    //std::vector<glm::vec3> newVec;
    //this->verts = newVec;

    for(int i = 0; i < this->vertCount; i++ ) {
        this->verts.push_back( copy[i]);
    }
}

float& entity::operator[](const int index) {
    return this->verts[index];
}

float* entity::getVertices() {
    return this->verts.data();
}

unsigned int* entity::getIndices() {
    return this->indicies.data();
}


int entity::getVertCount() {
    return this->vertCount;
}

int entity::getIndexCount() {
    return this->indexCount;
}

Entity Entity::createEntity() {
    static id_t currentId = 0;
    return Entity{currentId++};
}

Entity::Entity(id_t objectId) : id{objectId} {}

void Entity::loadObj(std::string path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        std::cerr << "❌ Failed to load OBJ file: " << path << "\n" << err << std::endl;
        return;
    }

    vertices.clear();
    indices.clear();

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);
            indices.push_back(static_cast<unsigned int>(indices.size()));
        }
    }

    std::cout << "🟢 Loaded " << vertices.size() / 3 << " vertices from " << path << std::endl;

    setupMesh();
}


void Entity::setupMesh() {
    if (VAO != 0) return;  // Prevent multiple buffer creations

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Entity::draw() {
    if (VAO == 0) {
        std::cerr << "❌ Error: VAO not initialized for entity!" << std::endl;
        return;
    }

    std::cout << "🎨 Rendering entity with VAO: " << VAO 
              << " and " << indices.size() << " indices.\n";

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
