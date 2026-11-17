class PerformanceEngine {
    constructor() {
        this.active = true;
        this.tasks = new Set();
        this.lastFrame = 0;
        this.frameInterval = 1000 / 60;
    }

    // Controla a frequência de execução
    throttle(callback, delay = 16) {
        let lastCall = 0;

        return (...args) => {
            const now = performance.now();

            if (now - lastCall >= delay) {
                lastCall = now;
                callback(...args);
            }
        };
    }

    // Evita executar tarefas em segundo plano
    checkVisibility() {
        this.active = !document.hidden;
    }

    // Registra tarefas
    addTask(callback) {
        this.tasks.add(callback);
    }

    // Executa tarefas registradas
    update() {
        if (!this.active) return;

        this.tasks.forEach(task => task());
    }

    // Limpa tarefas temporárias
    clearTasks() {
        this.tasks.clear();
    }

    // Inicializa o sistema
    start() {
        document.addEventListener(
            "visibilitychange",
            () => this.checkVisibility()
        );

        const loop = () => {
            if (this.active) {
                this.update();
            }

            requestAnimationFrame(loop);
        };

        requestAnimationFrame(loop);
    }
}

const engine = new PerformanceEngine();

engine.start();