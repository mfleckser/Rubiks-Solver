#include "Meshes.h"

Cube::Cube() {
    state = {SOLVED_CORNERS, SOLVED_EDGES};

    global = MatrixIdentity();
    
    cubeMesh = GenMeshCube(1, 1, 1);
    faceMesh = GenMeshCube(0.9, 0.9, 0.1);
    
    cubeMat = LoadMaterialDefault();
    cubeMat.maps[MATERIAL_MAP_DIFFUSE].color = BLACK;
    
    for (int c = 0; c < 6; c++) {
        faceMats[c] = LoadMaterialDefault();
        faceMats[c].maps[MATERIAL_MAP_DIFFUSE].color = SIDE_COLORS[c];
    }
    
    for (int i = -1; i <= 1; i++)
    for (int j = -1; j <= 1; j++)
    for (int k = -1; k <= 1; k++)
    if (i != 0 || j != 0 || k != 0) {
        blocks.push_back(Block(i, j, k));
    }
}

void Cube::draw() {
    for (Block& b : blocks) {
        DrawMesh(cubeMesh, cubeMat, MatrixMultiply(b.cube, global));
        for (Face& f : b.faces) {
            Matrix m = MatrixMultiply(f.face, b.cube);
            DrawMesh(faceMesh, faceMats[f.side], MatrixMultiply(m, global));
        }
    }
}

void Cube::rotateYaw(float angle) {
    Vector3 up = Vector3Transform({0, 1, 0}, global);
    Matrix rotation = MatrixRotate(up, angle);
    global = MatrixMultiply(global, rotation);
}

void Cube::rotatePitch(float angle) {
    Matrix rotation = MatrixRotate({1, 0, 0}, angle);
    global = MatrixMultiply(global, rotation);
}

std::vector<Block*> Cube::getSide(Side side) {
    std::vector<Block*> sideBlocks;
    Vector3 norm = SIDE_NORMS[side];

    for (Block& b : blocks) {
        float dot = Vector3DotProduct(norm, b.getPosition());
        if (dot >= 0.999) {
            sideBlocks.push_back(&b);
        }
    }

    return sideBlocks;
}

void Cube::updateState(Side side, int dir) {
    state = make_move(state, {side, dir});
}

void Cube::makeMove(Side side, int dir) {
    std::vector<Block*> blocks = getSide(side);
    Vector3 axis = Vector3Scale(SIDE_NORMS[side], dir);

    for (Block* b : blocks) {
        b->cube = MatrixMultiply(b->cube, MatrixRotate(axis, -PI/2));
    }

    updateState(side, dir);
}

Face::Face(Side side) : side(side) {
    switch (side) {
        case TOP:
            face = MatrixMultiply(MatrixRotateX(PI/2), MatrixTranslate(0, 0.5, 0));
            break;
        case BOTTOM:
            face = MatrixMultiply(MatrixRotateX(PI/2), MatrixTranslate(0, -0.5, 0));
            break;
        case RIGHT:
            face = MatrixMultiply(MatrixRotateY(PI/2), MatrixTranslate(0.5, 0, 0));
            break;
        case LEFT:
            face = MatrixMultiply(MatrixRotateY(PI/2), MatrixTranslate(-0.5, 0, 0));
            break;
        case FRONT:
            face = MatrixTranslate(0, 0, 0.5);
            break;
        case BACK:
            face = MatrixTranslate(0, 0, -0.5);
            break;
    }
}

Block::Block(int i, int j, int k) {
    cube = MatrixTranslate(i, j, k);
    if (i == 1) {
        faces.push_back(Face(RIGHT));
    } else if (i == -1) {
        faces.push_back(Face(LEFT));
    }

    if (j == 1) {
        faces.push_back(Face(TOP));
    } else if (j == -1) {
        faces.push_back(Face(BOTTOM));
    }

    if (k == 1) {
        faces.push_back(Face(FRONT));
    } else if (k == -1) {
        faces.push_back(Face(BACK));
    }
}

Vector3 Block::getPosition() {
    return Vector3Transform({0, 0, 0}, cube);
}
