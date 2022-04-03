#include <iostream>
#include <ostream>
#include <map>
#include <vector>
#include <array>
#include <queue>
#include <unordered_map>

#define N 3

enum Color{
    WHITE, YELLOW, RED, BLUE, ORANGE, GREEN
};

enum Move{
    R, Ri, L, Li, B, Bi, D, Di, F, Fi, U, Ui,NOP
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

std::ostream& operator<<(std::ostream& out, const Move& move){
    switch(move){
    case R:     out << "R";     break;
    case Ri:    out << "Ri";    break;
    case L:     out << "L";     break;
    case Li:    out << "Li";    break;
    case B:     out << "B";     break;
    case Bi:    out << "Bi";    break;
    case D:     out << "D";     break;
    case Di:    out << "Di";    break;
    case F:     out << "F";     break;
    case Fi:    out << "Fi";    break;
    case U:     out << "U";     break;
    case Ui:    out << "Ui";    break;
    case NOP:                   break;
    }
    return out;
}

struct Rubik3{
    std::map<Face, std::vector<Color>> cube;

    Rubik3();
    Rubik3& operator=(Rubik3 r);
    void show();
    void show_face(Face faceId);
    void unfold_and_show();
    Rubik3 next(Move move);
    bool is_identity();
    bool operator==(const Rubik3& r);
    bool solve(std::unordered_map<const char*, unsigned int>&);
    void shuffle(int cnt = 10);
};

Rubik3::Rubik3(){
    cube[FRONT].assign(N * N, GREEN);
    cube[LEFT].assign(N * N, ORANGE);
    cube[RIGHT].assign(N * N, RED);
    cube[BACK].assign(N * N, BLUE);
    cube[UP].assign(N * N, WHITE);
    cube[DOWN].assign(N * N, YELLOW);
}

Rubik3& Rubik3::operator=(Rubik3 r){
    cube = r.cube;
    return *this;
}

void Rubik3::show_face(Face faceId){
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            std::cerr << cube[faceId][i * N + j] << ' ';
        }
        std::cerr << '\n';
    }

    std::cerr << '\n';

}

void Rubik3::show(){
    for (auto face = Face::FRONT; face <= Face::DOWN;){
        std::cerr << face << '\n';
        show_face(face);
        face = static_cast<Face>(static_cast<int>(face) + 1);
    }
}

void Rubik3::unfold_and_show(){
    auto print_space = [](int n){
        for (int i = 0; i < n; ++i) std::cerr << ' ';
    };

    auto endl = [](){
        std::cerr << '\n';
    };

    // print UP face
    for (int i = 0; i < N; ++i){
        print_space(6);
        for (int j = 0; j < N; ++j)
            std::cerr << cube[UP][i * N + j] << ' ';
        endl();
    }

    // print left, front, right, back faces
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j)
            std::cerr << cube[LEFT][i * N + j] << ' ';
        for (int j = 0; j < N; ++j)
            std::cerr << cube[FRONT][i * N + j] << ' ';
        for (int j = 0; j < N; ++j)
            std::cerr << cube[RIGHT][i * N + j] << ' ';
        for (int j = 0; j < N; ++j)
            std::cerr << cube[BACK][i * N + j] << ' ';
        endl();
    }

    // print DOWN face
    for (int i = 0; i < N; ++i){
        print_space(6);
        for (int j = 0; j < N; ++j)
            std::cerr << cube[DOWN][i * N + j] << ' ';
        endl();
    }
    endl();
}

Rubik3 Rubik3::next(Move move){
    Rubik3 new_rubik(*this);

    // Define a lambda to rotate the edge pieces.
    // f1 is the dest face and f2 is the source face.
    auto rotate_edge_pieces = 
        [&](Face f1, std::array<int, 3> f1Idx, 
        Face f2, std::array<int, 3> f2Idx){
            new_rubik.cube[f1][f1Idx[0]] = cube[f2][f2Idx[0]];
            new_rubik.cube[f1][f1Idx[1]] = cube[f2][f2Idx[1]];
            new_rubik.cube[f1][f1Idx[2]] = cube[f2][f2Idx[2]];
        };

    // Nested block to perform face rotation
    {
        // define even and odd subgroups
        int even[] = {0, 2, 8, 6};
        int odd[] = {1, 5, 7, 3};

        // 'invert' the subgroups
        if (move == Fi or move == Ri or move == Li or 
            move == Bi or move == Ui or move == Di){
            std::swap(even[1], even[3]);
            std::swap(odd[1], odd[3]);
        }

        // determine face to rotate
        Face face;
        switch(move){
        case R:case Ri:    face = RIGHT;   break;
        case L:case Li:    face = LEFT;    break;
        case B:case Bi:    face = BACK;    break;
        case D:case Di:    face = DOWN;    break;
        case F:case Fi:    face = FRONT;   break;
        case U:case Ui:    face = UP;      break;
        case NOP:
          break;
        }

        // assign the colors
        for (int i = 0; i+1 < 4; ++i){
            new_rubik.cube[face][even[i+1]] = cube[face][even[i]];
            new_rubik.cube[face][odd[i+1]] = cube[face][odd[i]];
        }
        new_rubik.cube[face][even[0]] = cube[face][even[3]];
        new_rubik.cube[face][odd[0]] = cube[face][odd[3]];
    }
    
    if (move == Move::F){
        rotate_edge_pieces(RIGHT, {0,3,6}, UP, {6,7,8});
        rotate_edge_pieces(DOWN, {0,1,2}, RIGHT, {0,3,6});
        rotate_edge_pieces(LEFT, {2,5,8}, DOWN, {0,1,2});
        rotate_edge_pieces(UP, {6,7,8}, LEFT, {2,5,8});
    }
    else if (move == Fi){
        rotate_edge_pieces(LEFT, {2,5,8}, UP, {6,7,8});
        rotate_edge_pieces(UP, {6,7,8}, RIGHT, {0,3,6});
        rotate_edge_pieces(RIGHT, {0,3,6}, DOWN, {0,1,2});
        rotate_edge_pieces(DOWN, {0,1,2}, LEFT, {2,5,8});
    }
    else if (move == B){
        rotate_edge_pieces(LEFT, {0,3,6}, UP, {0,1,2});
        rotate_edge_pieces(UP, {0,1,2}, RIGHT, {2,5,8});
        rotate_edge_pieces(RIGHT, {2,5,8}, DOWN, {6,7,8});
        rotate_edge_pieces(DOWN, {6,7,8}, LEFT, {0,3,6});
    }
    else if (move == Bi){
        rotate_edge_pieces(RIGHT, {2,5,8}, UP, {0,1,2});
        rotate_edge_pieces(LEFT, {0,3,6}, DOWN, {6,7,8});
        rotate_edge_pieces(UP, {2,1,0}, LEFT, {0,3,6});
        rotate_edge_pieces(DOWN, {8,7,6}, RIGHT, {2,5,8});
    }
    else if (move == L){
        rotate_edge_pieces(FRONT, {0,3,6}, UP, {0,3,6});
        rotate_edge_pieces(DOWN, {0,3,6}, FRONT, {0,3,6});
        rotate_edge_pieces(BACK, {2,5,8}, DOWN, {0,3,6});
        rotate_edge_pieces(UP, {0,3,6}, BACK, {8,5,2});
    }
    else if (move == Li){
        rotate_edge_pieces(BACK, {8,5,2}, UP, {0,3,6});
        rotate_edge_pieces(DOWN, {0,3,6}, BACK, {2,5,8});
        rotate_edge_pieces(FRONT, {0,3,6}, DOWN, {0,3,6});
        rotate_edge_pieces(UP, {0,3,6}, FRONT, {0,3,6});
    }
    else if (move == R){
        rotate_edge_pieces(BACK, {6,3,0}, UP, {2,5,8});
        rotate_edge_pieces(DOWN, {2,5,8}, BACK, {0,3,6});
        rotate_edge_pieces(FRONT, {8,5,2}, DOWN, {2,5,8});
        rotate_edge_pieces(UP, {2,5,8}, FRONT, {2,5,8});
    }
    else if (move == Ri){
        rotate_edge_pieces(FRONT, {2,5,8}, UP, {2,5,8});
        rotate_edge_pieces(DOWN, {2,5,8}, FRONT, {2,5,8});
        rotate_edge_pieces(BACK, {6,3,0}, DOWN, {2,5,8});
        rotate_edge_pieces(UP, {8,5,2}, BACK, {0,3,6});
    }
    else if (move == U){
        rotate_edge_pieces(LEFT, {0,1,2}, FRONT, {0,1,2});
        rotate_edge_pieces(BACK, {0,1,2}, LEFT, {0,1,2});
        rotate_edge_pieces(RIGHT, {0,1,2}, BACK, {0,1,2});
        rotate_edge_pieces(FRONT, {0,1,2}, RIGHT, {0,1,2});
    }
    else if (move == Ui){
        rotate_edge_pieces(FRONT, {0,1,2}, LEFT, {0,1,2});
        rotate_edge_pieces(LEFT, {0,1,2}, BACK, {0,1,2});
        rotate_edge_pieces(BACK, {0,1,2}, RIGHT, {0,1,2});
        rotate_edge_pieces(RIGHT, {0,1,2}, FRONT, {0,1,2});
    }
    else if (move == D){
        rotate_edge_pieces(RIGHT, {6,7,8}, FRONT, {6,7,8});
        rotate_edge_pieces(BACK, {6,7,8}, RIGHT, {6,7,8});
        rotate_edge_pieces(LEFT, {6,7,8}, BACK, {6,7,8});
        rotate_edge_pieces(FRONT, {6,7,8}, LEFT, {6,7,8});
    }
    else if (move == Di){
        rotate_edge_pieces(FRONT, {6,7,8}, RIGHT, {6,7,8});
        rotate_edge_pieces(RIGHT, {6,7,8}, BACK, {6,7,8});
        rotate_edge_pieces(BACK, {6,7,8}, LEFT, {6,7,8});
        rotate_edge_pieces(LEFT, {6,7,8}, FRONT, {6,7,8});
    }
    #ifdef DEBUG
    // std::cerr << "Action: " << move << '\n';
    // new_rubik.unfold_and_show();
    #endif
    return new_rubik;
}

bool Rubik3::is_identity(){
    auto all_same = [this](Face f) -> bool {
        auto c = cube[f][0];
        for (int i = 1; i < N*N; ++i)
            if (cube[f][i] != c)
                return false;
        return true;
    };

    for (const auto& [face, v] : cube)
        if (!all_same(face))
            return false;
    return true;
}

bool Rubik3::operator==(const Rubik3& r){
    return cube == r.cube;
}

void Rubik3::shuffle(int cnt){
    auto random_move = []() -> Move {
        return static_cast<Move>(rand() % ((int)Ui + 1));
    };

    while (cnt--)
        *this = next(random_move());
}

typedef std::pair<Rubik3, Move> State;
bool Rubik3::solve(std::unordered_map<const char*, unsigned int>& stats){
    auto reverting_moves = [](Move m1, Move m2) -> bool {
        if (m1 == F and m2 == Fi)   return true;
        if (m1 == Fi and m2 == F)   return true;
        if (m1 == B and m2 == Bi)   return true;
        if (m1 == Bi and m2 == B)   return true;
        if (m1 == L and m2 == Li)   return true;
        if (m1 == Li and m2 == L)   return true;
        if (m1 == R and m2 == Ri)   return true;
        if (m1 == Ri and m2 == R)   return true;
        if (m1 == U and m2 == Ui)   return true;
        if (m1 == Ui and m2 == U)   return true;
        if (m1 == D and m2 == Di)   return true;
        if (m1 == Di and m2 == D)   return true;
        return false;
    };

    // initialize BFS containers
    std::array<Move,12> actions(
        {R, Ri, L, Li, B, Bi, D, Di, F, Fi, U, Ui});
    std::queue<State> q;
    q.push({*this, NOP});

    while (!q.empty()){
        auto [curr, last_action] = q.front(); q.pop();
        stats["nodes visited"]++;

        if (curr.is_identity())
            return true;

        for (auto& action : actions){
            Rubik3 next = curr.next(action);
            if (reverting_moves(action, last_action))
                continue;
            q.push({next, action});
            stats["nodes expanded"]++;
        }
    }
    return false;
}

int main(){
    // summary metrics
    std::unordered_map<const char*, unsigned int> stats{
        {"level", 0},
        {"nodes expanded", 0},
        {"nodes visited", 0}
    };
    Rubik3 cube;
    cube.shuffle(5);
    cube.unfold_and_show();
    std::cout << (cube.solve(stats) ? "SOLVED!" : "UNSOLVED!") << '\n';
    
    std::cout << "\n============================\nSUMMARY:\n";
    for (auto& [k,v] : stats)
        std::cout << k << " : " << v << '\n';
    std::cout << "============================\n";
    
    return 0;
}
