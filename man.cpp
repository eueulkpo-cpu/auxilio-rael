#include <iostream>

struct Mira {
    float sensibilidade;
    float precisao;
    float alvoX;
    float alvoY;
};

int main() {

    Mira mira{
        1.0f,
        50.0f,
        100.0f,
        50.0f
    };

    // Referências aos valores da nossa própria estrutura
    float* sensi = &mira.sensibilidade;
    float* precisao = &mira.precisao;
    float* x = &mira.alvoX;
    float* y = &mira.alvoY;

    std::cout << "=== LABORATORIO DE MIRA ===\n\n";

    std::cout << "Sensibilidade: "
              << *sensi << "\n";

    std::cout << "Precisao atual: "
              << *precisao << "%\n";

    std::cout << "Posicao do alvo: ("
              << *x << ", " << *y << ")\n\n";

    // Alterando os valores através dos ponteiros
    *sensi = 2.5f;
    *precisao = 95.0f;
    *x = 250.0f;
    *y = 120.0f;

    std::cout << "=== DEPOIS DA ALTERACAO ===\n\n";

    std::cout << "Sensibilidade: "
              << mira.sensibilidade << "\n";

    std::cout << "Precisao: "
              << mira.precisao << "%\n";

    std::cout << "Posicao do alvo: ("
              << mira.alvoX << ", "
              << mira.alvoY << ")\n";

    return 0;
}