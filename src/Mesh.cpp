#include "Mesh.h" 

struct Material
{
    std::string name;
    glm::vec3 Kd;
    glm::vec3 Ks;
    float Ns;
};
//loads obj files, must be a triangle only mesh currently
void LoadOBJ(const std::string& fileName, std::vector <Vertex>& vertices, std::vector<GLuint> &indices)
{
    std::ifstream file(fileName);
    std::string line;

    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << fileName << std::endl;
        return;
    }

    std::string pos;
    std::string normals;
    std::string faces;

    std::string mltFileName;

    std::vector<std::string> faceMaterials;
    std::string currentMtl = "default";

    while (std::getline(file, line))
    {
        if(line.substr(0, 2) == "v ")
        {
            pos += line.substr(2) + '\n';
        }
        else if(line.substr(0, 2) == "vn")
        {
            normals += line.substr(2) + '\n';
        }
        else if(line.substr(0, 2) == "f ")
        {
            faces += line.substr(2) + '\n';
            faceMaterials.push_back(currentMtl);
        }
        else if(line.substr(0, 7) == "usemtl ")
        {
            currentMtl = line.substr(7);
        }
        else if(line.substr(0, 7) == "mtllib ")
        {
            mltFileName = line.substr(7);
        }
    }

    file.close();

    std::ifstream mtlFile(mltFileName);

    if (!mtlFile.is_open())
    {
        std::cout << "Failed to open file: " << mltFileName << std::endl;
        return;
    }
    
    std::unordered_map<std::string, Material> materials;
    std::string mtlName; 
    Material currentMaterial;

    currentMaterial.name = "default";
    currentMaterial.Kd = {0.8, 0.0, 0.8};
    currentMaterial.Ks = {0.5, 0.5, 0.5};
    currentMaterial.Ns = 32.0;
    materials["default"] = currentMaterial;

    while (std::getline(mtlFile, line))
    {
        if(line.substr(0, 7) == "newmtl ")
        {
            if(!mtlName.empty())           
            {
                materials[mtlName] = currentMaterial;
            }
            mtlName = line.substr(7);
            currentMaterial = Material();
            currentMaterial.name = mtlName;
        }
        else if(line.substr(0, 3) == "Kd ")
        {
            std::string junk;
            float v1, v2, v3;
            std::istringstream KdStream(line);
            KdStream >> junk;
            KdStream >> v1;
            KdStream >> v2;
            KdStream >> v3;
            currentMaterial.Kd = {v1,v2,v3};
        }
        else if(line.substr(0, 3) == "Ks ")
        {
            std::string junk;
            float v1, v2, v3;
            std::istringstream KsStream(line);
            KsStream >> junk;
            KsStream >> v1;
            KsStream >> v2;
            KsStream >> v3;
            currentMaterial.Ks = {v1,v2,v3};
        }
        else if(line.substr(0, 3) == "Ns ")
        {
            std::string junk;
            float v;
            std::istringstream NsStream(line);
            NsStream >> junk;
            NsStream >> v;
            currentMaterial.Ns = v;
        }
    }

    if(!mtlName.empty())
    {
        materials[mtlName] = currentMaterial;
    }

    mtlFile.close();

    std::istringstream PosStream(pos);
    std::istringstream NormalsStream(normals);
    std::istringstream FaceStream(faces);
    
    std::vector <glm::vec3> positions;
    float f;
    int i = 0;
    glm::vec3 position;
    while (PosStream >> f)
    {
        position[0] = f;
        PosStream >> f;
        position[1] = f;
        PosStream >> f;
        position[2] = f;
        positions.push_back(position);
    }

    std::vector <glm::vec3> norms;
    glm::vec3 normal;
    while (NormalsStream >> f)
    {
        normal[0] = f;
        NormalsStream >> f;
        normal[1] = f;
        NormalsStream >> f;
        normal[2] = f;
        norms.push_back(normal);
    }


    int triangleCount = 0;
    
    //change to line based parsing later if allowing quads or optimization long term
    GLuint p1, p2, p3, n1, n2, n3;
    std::string sub1, sub2;
    std::string face;
    while (FaceStream >> face)
    {
        p1 = std::stoi(face.substr(0, face.find('/'))) - 1; //pos
        sub1 = face.substr(face.find('/') + 1); // uv not used
        sub2 = sub1.substr(sub1.find('/') + 1); //normal
        n1 = std::stoi(sub2) - 1; 

        FaceStream >> face;
        p2 = std::stoi(face.substr(0, face.find('/'))) - 1;
        sub1 = face.substr(face.find('/') + 1); // uv not used
        sub2 = sub1.substr(sub1.find('/') + 1); //normal
        n2 = std::stoi(sub2) - 1; 

        FaceStream >> face;
        p3 = std::stoi(face.substr(0, face.find('/'))) - 1;
        sub1 = face.substr(face.find('/') + 1); // uv not used
        sub2 = sub1.substr(sub1.find('/') + 1); //normal
        n3 = std::stoi(sub2) - 1;

        int faceIndex = triangleCount / 2; // 2 triangles per face
        //glm::vec3 col = faceColors[faceIndex % 6];
        glm::vec3 col = materials[faceMaterials[triangleCount]].Kd;
        glm::vec3 specColor = materials[faceMaterials[triangleCount]].Ks;
        float shininess = materials[faceMaterials[triangleCount]].Ns;
        

        // Create vertices
        vertices.emplace_back(positions[p1], norms[n1], col, specColor, shininess);
        vertices.emplace_back(positions[p2], norms[n2], col, specColor, shininess);
        vertices.emplace_back(positions[p3], norms[n3], col, specColor, shininess);

        // Create indices pointing to those vertices
        indices.push_back(vertices.size() - 3);
        indices.push_back(vertices.size() - 2);
        indices.push_back(vertices.size() - 1);

        triangleCount++;
    }
}


Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices) 
{ 
        if(!indices.empty())
        {
            indexed = true;
            drawCount = static_cast<int>(indices.size());
        }
        else
        {
            drawCount = static_cast<int>(vertices.size()); //3 floats per vertex
        }

        //generates VAO and VBO objects 
        glGenVertexArrays(1, &VAO); 
        glGenBuffers(1, &VBO); 
        if(indexed){glGenBuffers(1, &EBO);}

        //VAO
        //bind (select) object 
        glBindVertexArray(VAO); 

        //VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO); 
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW); //upload vertex data to GPU 

        //EBO
        if(indexed)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        }

        
        //Attribute: 0 
        //3 floats per vertex 
        //No normalization 
        //Stride = 3 floats 
        //Offset = 0 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, specColor));
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, shininess));
        glEnableVertexAttribArray(4);
        
        //unbinds 
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0); 
    } 
    
    Mesh::~Mesh() 
    { 
        if (VBO != 0) 
        { 
            glDeleteBuffers(1, &VBO); 
        } 
        if (VAO != 0) 
        { 
            glDeleteVertexArrays(1, &VAO); 
        } 
        if (EBO != 0) 
        { 
            glDeleteBuffers(1, &EBO); 
        } 
    } 
    void Mesh::Draw() const 
    { 
        glBindVertexArray(VAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(indexed)
        { 
            glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, drawCount);
        } 
        glBindVertexArray(0); 
    }