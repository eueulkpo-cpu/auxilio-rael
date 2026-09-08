// ============================================================
// SISTEMA DE MIRA ASSISTIDA COMPLETO - HEAD-LOCK FILTER
// ============================================================
// Este código pode ser usado em:
// - Mod Menus para Android (via JNI)
// - Injeção em APKs (via bibliotecas .so)
// - Emuladores (PC) com leitura de memória
// ============================================================

#include <cmath>
#include <random>
#include <thread>
#include <chrono>

// ============================================================
// 1. ESTRUTURAS DE DADOS
// ============================================================

struct Vector3 {
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    
    float Distance(const Vector3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return sqrt(dx*dx + dy*dy + dz*dz);
    }
};

struct Player {
    Vector3 headPosition;  // Posição da cabeça (hitbox)
    Vector3 velocity;      // Velocidade (para predição)
    bool isAlive;          // Vivo ou morto
    int teamId;            // Time (para não mirar em aliados)
    float health;          // Vida (para filtrar mortos)
};

struct ViewAngles {
    float pitch;  // Vertical (cima/baixo)
    float yaw;    // Horizontal (esquerda/direita)
    float roll;   // Inclinação (geralmente 0)
};

// ============================================================
// 2. CLASSE PRINCIPAL DO AIMBOT COM HEAD-LOCK
// ============================================================

class AimbotHeadLock {
private:
    // Configurações do aimbot
    float SMOOTHING = 0.25f;        // 0 = instantâneo, 1 = super suave
    float MAX_HEAD_OFFSET = 0.5f;   // Margem máxima da cabeça (graus)
    float MAX_FOV = 15.0f;          // Campo de visão para ativação
    float MAX_DISTANCE = 1000.0f;   // Distância máxima de ativação
    float BULLET_DROP = 0.5f;       // Compensação de queda
    float BULLET_SPEED = 800.0f;    // Velocidade da bala (unidades/s)
    
    // Gerador de jitter (tremor natural)
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> jitterDist;
    float JITTER_AMOUNT = 0.08f;    // Amplitude do tremor (graus)
    
public:
    AimbotHeadLock() : gen(rd()), jitterDist(-JITTER_AMOUNT, JITTER_AMOUNT) {}
    
    // ============================================================
    // 3. FUNÇÃO PRINCIPAL - CHAMADA A CADA FRAME
    // ============================================================
    void RunFrame(Player& localPlayer, std::vector<Player>& enemies) {
        // 1. Encontra o melhor alvo (mais próximo do centro da tela)
        Player* target = FindBestTarget(localPlayer, enemies);
        if (!target) return;
        
        // 2. Verifica se a cabeça está visível (não atrás de parede)
        if (!IsHeadVisible(localPlayer.headPosition, target->headPosition)) {
            return;  // Cabeça oculta → não ativa a mira
        }
        
        // 3. Calcula o ângulo APENAS para a cabeça (com predição)
        ViewAngles targetAngle = CalculateHeadAngle(localPlayer, *target);
        
        // 4. Obtém o ângulo atual da câmera
        ViewAngles currentAngle = GetCurrentViewAngle();
        
        // 5. --- FILTRO HEAD-LOCK: impede a mira de passar da cabeça ---
        ViewAngles lockedAngle = ApplyHeadLockFilter(currentAngle, targetAngle);
        
        // 6. Aplica suavização (smoothing) para movimento natural
        ViewAngles smoothAngle = SmoothAngle(currentAngle, lockedAngle);
        
        // 7. Adiciona jitter (tremor humano) e aplica no jogo
        ViewAngles finalAngle = AddJitter(smoothAngle);
        ApplyViewAngle(finalAngle);
    }
    
private:
    // ============================================================
    // 4. SELEÇÃO DO MELHOR ALVO
    // ============================================================
    Player* FindBestTarget(Player& local, std::vector<Player>& enemies) {
        Player* bestTarget = nullptr;
        float bestScore = 9999.0f;
        
        ViewAngles currentAngle = GetCurrentViewAngle();
        
        for (auto& enemy : enemies) {
            // Filtra inimigos inválidos
            if (!enemy.isAlive || enemy.health <= 0) continue;
            if (enemy.teamId == local.teamId) continue;  // Não mira em aliados
            
            // Calcula distância
            float distance = local.headPosition.Distance(enemy.headPosition);
            if (distance > MAX_DISTANCE) continue;
            
            // Calcula ângulo para a cabeça do inimigo
            Vector3 delta = enemy.headPosition - local.headPosition;
            ViewAngles angleToHead = CalculateAngleFromDelta(delta);
            
            // Calcula o FOV (distância angular da mira atual)
            float fov = CalculateFOV(currentAngle, angleToHead);
            if (fov > MAX_FOV) continue;  // Fora do campo de visão
            
            // Score: prioriza FOV (mais importante) e distância
            float score = (fov / MAX_FOV) * 0.7f + (distance / MAX_DISTANCE) * 0.3f;
            
            if (score < bestScore) {
                bestScore = score;
                bestTarget = &enemy;
            }
        }
        
        return bestTarget;
    }
    
    // ============================================================
    // 5. CÁLCULO DE ÂNGULO COM PREDIÇÃO E BULLET DROP
    // ============================================================
    ViewAngles CalculateHeadAngle(Player& local, Player& target) {
        // --- REGRA 1: Usa EXCLUSIVAMENTE a posição da cabeça ---
        Vector3 headPos = target.headPosition;
        
        // --- Predição de movimento (compensa alvo em movimento) ---
        float distance = local.headPosition.Distance(headPos);
        float timeToTarget = distance / BULLET_SPEED;
        
        Vector3 predictedPos = headPos + (target.velocity * timeToTarget);
        
        // --- Compensação de queda de projétil (bullet drop) ---
        float dropCompensation = distance * BULLET_DROP * 0.001f;
        predictedPos.z += dropCompensation;  // Eleva o ponto de mira
        
        // --- Calcula o ângulo para a posição prevista ---
        Vector3 delta = predictedPos - local.headPosition;
        return CalculateAngleFromDelta(delta);
    }
    
    // ============================================================
    // 6. TRIGONOMETRIA PARA CÁLCULO DE ÂNGULOS
    // ============================================================
    ViewAngles CalculateAngleFromDelta(const Vector3& delta) {
        ViewAngles angles;
        
        // Hipotenusa no plano horizontal (XZ)
        float hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
        
        // Yaw (horizontal) - em graus
        angles.yaw = atan2(delta.y, delta.x) * 180.0f / 3.14159265f;
        
        // Pitch (vertical) - em graus
        angles.pitch = -atan2(delta.z, hyp) * 180.0f / 3.14159265f;
        
        // Roll (inclinação) - geralmente 0 em FPS
        angles.roll = 0.0f;
        
        return angles;
    }
    
    // ============================================================
    // 7. CÁLCULO DE FOV (DISTÂNCIA ANGULAR)
    // ============================================================
    float CalculateFOV(const ViewAngles& current, const ViewAngles& target) {
        float deltaYaw = target.yaw - current.yaw;
        float deltaPitch = target.pitch - current.pitch;
        
        // Normaliza Yaw para -180 a 180
        while (deltaYaw > 180.0f) deltaYaw -= 360.0f;
        while (deltaYaw < -180.0f) deltaYaw += 360.0f;
        
        return sqrt(deltaPitch * deltaPitch + deltaYaw * deltaYaw);
    }
    
    // ============================================================
    // 8. VERIFICAÇÃO DE VISIBILIDADE (TRACERAY)
    // ============================================================
    bool IsHeadVisible(const Vector3& eyePos, const Vector3& headPos) {
        // --- Em um jogo real, aqui seria feito um raycast ---
        // Exemplo: Engine::TraceRay(eyePos, headPos, MASK_SHOT, &traceInfo);
        // return (traceInfo.hitEntity != nullptr);
        
        // --- SIMULAÇÃO para demonstração ---
        // 30% de chance de estar atrás de parede
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dist(0, 9);
        return (dist(gen) >= 3);  // 70% visível
    }
    
    // ============================================================
    // 9. FILTRO HEAD-LOCK (MIRA NÃO PASSA DA CABEÇA)
    // ============================================================
    ViewAngles ApplyHeadLockFilter(const ViewAngles& current, const ViewAngles& target) {
        ViewAngles locked = current;
        
        // Calcula a diferença entre ângulo atual e o alvo (cabeça)
        float deltaPitch = target.pitch - current.pitch;
        float deltaYaw = target.yaw - current.yaw;
        
        // Normaliza os deltas
        while (deltaYaw > 180.0f) deltaYaw -= 360.0f;
        while (deltaYaw < -180.0f) deltaYaw += 360.0f;
        while (deltaPitch > 180.0f) deltaPitch -= 360.0f;
        while (deltaPitch < -180.0f) deltaPitch += 360.0f;
        
        // --- FILTRO: Limita o movimento para NUNCA ultrapassar a cabeça ---
        // Se a diferença for maior que a margem permitida, corrige
        if (fabs(deltaPitch) > MAX_HEAD_OFFSET) {
            locked.pitch = current.pitch + (deltaPitch > 0 ? MAX_HEAD_OFFSET : -MAX_HEAD_OFFSET);
        } else {
            locked.pitch = current.pitch + deltaPitch;  // Dentro do limite, segue normal
        }
        
        if (fabs(deltaYaw) > MAX_HEAD_OFFSET) {
            locked.yaw = current.yaw + (deltaYaw > 0 ? MAX_HEAD_OFFSET : -MAX_HEAD_OFFSET);
        } else {
            locked.yaw = current.yaw + deltaYaw;  // Dentro do limite, segue normal
        }
        
        locked.roll = 0.0f;
        return locked;
    }
    
    // ============================================================
    // 10. SUAVIZAÇÃO (SMOOTHING)
    // ============================================================
    ViewAngles SmoothAngle(const ViewAngles& current, const ViewAngles& target) {
        ViewAngles smooth = current;
        
        // Calcula a diferença
        float deltaPitch = target.pitch - current.pitch;
        float deltaYaw = target.yaw - current.yaw;
        
        // Normaliza
        while (deltaYaw > 180.0f) deltaYaw -= 360.0f;
        while (deltaYaw < -180.0f) deltaYaw += 360.0f;
        
        // Aplica smoothing (move gradualmente)
        smooth.pitch = current.pitch + (deltaPitch * SMOOTHING);
        smooth.yaw = current.yaw + (deltaYaw * SMOOTHING);
        smooth.roll = 0.0f;
        
        return smooth;
    }
    
    // ============================================================
    // 11. JITTER (TREMOR NATURAL)
    // ============================================================
    ViewAngles AddJitter(const ViewAngles& angle) {
        ViewAngles jittered = angle;
        
        // Adiciona pequena variação aleatória (simula pulso humano)
        jittered.pitch += jitterDist(gen);
        jittered.yaw += jitterDist(gen);
        
        return jittered;
    }
    
    // ============================================================
    // 12. INTERFACE COM O JOGO (FUNÇÕES DE LEITURA/ESCRITA)
    // ============================================================
    
    // Lê o ângulo atual da câmera do jogador
    ViewAngles GetCurrentViewAngle() {
        // No mundo real: ler da memória do jogo
        // Exemplo no Android: 
        // float* viewAngles = (float*)0x12345678; // Offset encontrado
        // return { viewAngles[0], viewAngles[1], viewAngles[2] };
        
        // Simulação
        static ViewAngles current = {0.0f, 0.0f, 0.0f};
        return current;
    }
    
    // Aplica o ângulo no jogo (escrita de memória)
    void ApplyViewAngle(const ViewAngles& angle) {
        // No mundo real: escrever na memória do jogo
        // Exemplo no Android:
        // float* viewAngles = (float*)0x12345678; // Offset encontrado
        // viewAngles[0] = angle.pitch;
        // viewAngles[1] = angle.yaw;
        // viewAngles[2] = angle.roll;
        
        // Simulação: imprime no console
        printf("Ângulo aplicado: Pitch=%.2f, Yaw=%.2f\n", angle.pitch, angle.yaw);
    }
};

// ============================================================
// 13. EXEMPLO DE USO (LOOP PRINCIPAL)
// ============================================================
int main() {
    AimbotHeadLock aimbot;
    
    // Cria jogador local
    Player localPlayer;
    localPlayer.headPosition = Vector3(0, 0, 0);
    localPlayer.teamId = 1;
    localPlayer.isAlive = true;
    
    // Cria lista de inimigos
    std::vector<Player> enemies;
    for (int i = 0; i < 5; i++) {
        Player enemy;
        enemy.headPosition = Vector3(100 + i * 50, 20, 0);  // Posições diferentes
        enemy.velocity = Vector3(0.5f, 0, 0);  // Movimento para predição
        enemy.isAlive = true;
        enemy.teamId = 2;
        enemy.health = 100.0f;
        enemies.push_back(enemy);
    }
    
    // --- LOOP RODANDO A 60 FPS ---
    printf("Aimbot Head-Lock iniciado! Rodando a 60 FPS...\n");
    
    while (true) {
        aimbot.RunFrame(localPlayer, enemies);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // ~60 FPS
    }
    
    return 0;
}
