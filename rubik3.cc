#include <iostream>
#include <ostream>
#include <map>
#include <vector>

#define N 3

enum Color{
    WHITE, YELLOW, RED, BLUE, ORANGE, GREEN
};

enum Move{
    R, Ri, L, Li, B, Bi, D, Di, F, Fi, U, Ui
};

enum Face{
    FRONT, BACK, LEFT, RIGHT, UP, DOWN
};

std::ostream& operator<<(std::ostream& out, const Color& color){
    switch(color){
    case WHITE:     out << 'W'; break;
    case YELLOW:    out << 'Y'; break;
    case RED:       out << 'R'; break;
    case BLUE:      out << 'B'; break;
    case ORANGE:    out << 'O'; break;
    case GREEN:     out << 'G'; break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Face& face){
    switch(face){
    case FRONT:     out << "FRONT";     break;
    case BACK:      out << "BACK";      break;
    case LEFT:      out << "LEFT";      break;
    case RIGHT:     out << "RIGHT";     break;
    case UP:        out << "UP";       break;
    case DOWN:      out << "DOWN";    break;
    }
    return out;
}

struct Rubik3{
    std::map<Face, std::vector<Color>> cube;

    Rubik3();
    void show();
    void show_face(Face faceId);
    void unfold_and_show();
    Rubik3 next(Move move);
};

Rubik3::Rubik3(){
    cube[FRONT].assign(N * N, GREEN);
    cube[LEFT].assign(N * N, ORANGE);
    cube[RIGHT].assign(N * N, RED);
    cube[BACK].assign(N * N, BLUE);
    cube[UP].assign(N * N, WHITE);
    cube[DOWN].assign(N * N, YELLOW);
}

void Rubik3::show_face(Face faceId){
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            std::cout << cube[faceId][i * N + j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << '\n';

}

void Rubik3::show(){
    for (auto face = Face::FRONT; face <= Face::DOWN;){
        std::cout << face << '\n';
        show_face(face);
        face = static_cast<Face>(static_cast<int>(face) + 1);
    }
}

void Rubik3::unfold_and_show(){
    
    auto print_space = [](int n){
        for (int i = 0; i < n; ++i) std::cout << ' ';
    };

    auto endl = [](){
        std::cout << '\n';
    };

    // print UP face
    for (int i = 0; i < N; ++i){
        print_space(6);
        for (int j = 0; j < N; ++j)
            std::cout << cube[UP][i * N + j] << ' ';
        endl();
    }

    // print left, front, right, back faces
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j)
            std::cout << cube[LEFT][i * N + j] << ' ';
        for (int j = 0; j < N; ++j)
            std::cout << cube[FRONT][i * N + j] << ' ';
        for (int j = 0; j < N; ++j)
            std::cout << cube[RIGHT][i * N + j] << ' ';
        for (int j = 0; j < N; ++j)
            std::cout << cube[BACK][i * N + j] << ' ';
        endl();
    }

    // print DOWN face
    for (int i = 0; i < N; ++i){
        print_space(6);
        for (int j = 0; j < N; ++j)
            std::cout << cube[DOWN][i * N + j] << ' ';
        endl();
    }
    endl();
}

Rubik3 Rubik3::next(Move move){
    Rubik3 new_rubik = *this;    // copy this cube over
    if (move == Move::F){
        // translate squares in the front face
        // 0 -> 2 -> 8 -> 6
        // 1 -> 5 -> 7 -> 3
        int even[] = {0, 2, 8, 6};
        int odd[] = {1, 5, 7, 3};
        for (int i = 0; i+1 < 4; ++i){
            new_rubik.cube[FRONT][even[i+1]] = cube[FRONT][even[i]];
            new_rubik.cube[FRONT][odd[i+1]] = cube[FRONT][odd[i]];
        }
        new_rubik.cube[FRONT][even[0]] = cube[FRONT][even[3]];
        new_rubik.cube[FRONT][odd[0]] = cube[FRONT][odd[3]];

        // translate the edge squares
        // up -> right
        // 6,7,8 -> 0,3,6
        new_rubik.cube[RIGHT][0] = cube[UP][6];
        new_rubik.cube[RIGHT][3] = cube[UP][7];
        new_rubik.cube[RIGHT][6] = cube[UP][8];

        // right -> down
        // 0,3,6 -> 0,1,2
        new_rubik.cube[DOWN][0] = cube[RIGHT][0];
        new_rubik.cube[DOWN][1] = cube[RIGHT][3];
        new_rubik.cube[DOWN][2] = cube[RIGHT][6];

        // down -> left
        // 0,1,2 -> 2,5,8
        new_rubik.cube[LEFT][2] = cube[DOWN][0];
        new_rubik.cube[LEFT][5] = cube[DOWN][1];
        new_rubik.cube[LEFT][8] = cube[DOWN][2]; 
        
        // left -> up
        // 2,5,8 -> 6,7,8
        new_rubik.cube[UP][6] = cube[LEFT][2];
        new_rubik.cube[UP][7] = cube[LEFT][5];
        new_rubik.cube[UP][8] = cube[LEFT][8]; 
    }
    else if (move == Fi){
        // translate squares in the front face
        // 0 -> 6 -> 8 -> 2
        // 1 -> 3 -> 7 -> 5
        int even[] = {0, 6, 8, 2};
        int odd[] = {1, 3, 7, 5};
        for (int i = 0; i+1 < 4; ++i){
            new_rubik.cube[FRONT][even[i+1]] = cube[FRONT][even[i]];
            new_rubik.cube[FRONT][odd[i+1]] = cube[FRONT][odd[i]];
        }
        new_rubik.cube[FRONT][even[0]] = cube[FRONT][even[3]];
        new_rubik.cube[FRONT][odd[0]] = cube[FRONT][odd[3]];

        // translate the edge squares
        // up -> left
        // 6,7,8 -> 2,5,8
        new_rubik.cube[LEFT][2] = cube[UP][6];
        new_rubik.cube[LEFT][5] = cube[UP][7];
        new_rubik.cube[LEFT][8] = cube[UP][8];

        // right -> up
        // 0,3,6 -> 6,7,8
        new_rubik.cube[UP][6] = cube[RIGHT][0];
        new_rubik.cube[UP][7] = cube[RIGHT][3];
        new_rubik.cube[UP][8] = cube[RIGHT][6];

        // down -> right
        // 0,1,2 -> 0,3,6
        new_rubik.cube[RIGHT][0] = cube[DOWN][0];
        new_rubik.cube[RIGHT][3] = cube[DOWN][1];
        new_rubik.cube[RIGHT][6] = cube[DOWN][2]; 
        
        // left -> down
        // 2,5,8 -> 0,1,2
        new_rubik.cube[DOWN][0] = cube[LEFT][2];
        new_rubik.cube[DOWN][1] = cube[LEFT][5];
        new_rubik.cube[DOWN][2] = cube[LEFT][8]; 
    }
    else if (move == B){
        // rotate back face
        // 0 -> 2 -> 8 -> 6
        // 1 -> 5 -> 7 -> 3
        int even[] = {0, 2, 8, 6};
        int odd[] = {1, 5, 7, 3};
        for (int i = 0; i+1 < 4; ++i){
            new_rubik.cube[BACK][even[i+1]] = cube[BACK][even[i]];
            new_rubik.cube[BACK][odd[i+1]] = cube[BACK][odd[i]];
        }
        new_rubik.cube[BACK][even[0]] = cube[BACK][even[3]];
        new_rubik.cube[BACK][odd[0]] = cube[BACK][odd[3]];

        // translate the edge squares
        // up -> left
        // 0,1,2 -> 0,3,6
        new_rubik.cube[LEFT][0] = cube[UP][0];
        new_rubik.cube[LEFT][3] = cube[UP][1];
        new_rubik.cube[LEFT][6] = cube[UP][2];

        // right -> up
        // 2,5,8 -> 0,1,2
        new_rubik.cube[UP][0] = cube[RIGHT][2];
        new_rubik.cube[UP][1] = cube[RIGHT][5];
        new_rubik.cube[UP][2] = cube[RIGHT][8];

        // down -> right
        // 6,7,8 -> 2,5,8
        new_rubik.cube[RIGHT][2] = cube[DOWN][6];
        new_rubik.cube[RIGHT][5] = cube[DOWN][7];
        new_rubik.cube[RIGHT][8] = cube[DOWN][8]; 
        
        // left -> down
        // 0,3,6 -> 6,7,8
        new_rubik.cube[DOWN][6] = cube[LEFT][0];
        new_rubik.cube[DOWN][7] = cube[LEFT][3];
        new_rubik.cube[DOWN][8] = cube[LEFT][6]; 
    }
    return new_rubik;
}

int main(){
    Rubik3 cube;
    // cube.show();
    cube.unfold_and_show();
    cube.next(B).unfold_and_show();
    return 0;
}
