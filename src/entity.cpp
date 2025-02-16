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
        std::cerr << "Failed to load OBJ file: " << path << "\n" << err << std::endl;
        return;
    }

    vertices.clear();
    indices.clear();

    for (const auto& shape : shapes) {
        for (size_t i = 0; i < shape.mesh.indices.size(); i += 3) {
            tinyobj::index_t idx0 = shape.mesh.indices[i];
            tinyobj::index_t idx1 = shape.mesh.indices[i + 1];
            tinyobj::index_t idx2 = shape.mesh.indices[i + 2];

            // Get vertex positions
            glm::vec3 v0 = glm::vec3(
                attrib.vertices[3 * idx0.vertex_index + 0],
                attrib.vertices[3 * idx0.vertex_index + 1],
                attrib.vertices[3 * idx0.vertex_index + 2]);

            glm::vec3 v1 = glm::vec3(
                attrib.vertices[3 * idx1.vertex_index + 0],
                attrib.vertices[3 * idx1.vertex_index + 1],
                attrib.vertices[3 * idx1.vertex_index + 2]);

            glm::vec3 v2 = glm::vec3(
                attrib.vertices[3 * idx2.vertex_index + 0],
                attrib.vertices[3 * idx2.vertex_index + 1],
                attrib.vertices[3 * idx2.vertex_index + 2]);

            // Compute face normal
            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            // Store interleaved position and normal
            vertices.insert(vertices.end(), { v0.x, v0.y, v0.z, normal.x, normal.y, normal.z });
            vertices.insert(vertices.end(), { v1.x, v1.y, v1.z, normal.x, normal.y, normal.z });
            vertices.insert(vertices.end(), { v2.x, v2.y, v2.z, normal.x, normal.y, normal.z });

            // Store indices
            indices.push_back(indices.size());
            indices.push_back(indices.size());
            indices.push_back(indices.size());
        }
    }

    setupMesh();
}

void Entity::setupMesh() {
    if (VAO != 0) return; // Prevent multiple buffer creations

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Vertex Positions (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex Normals (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Entity::draw() {
    if (VAO == 0) {
        std::cerr << "❌ Error: VAO not initialized for entity!" << std::endl;
        return;
    }

    //std::cout << "🎨 Rendering entity with VAO: " << VAO 
              //<< " and " << indices.size() << " indices.\n";

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
