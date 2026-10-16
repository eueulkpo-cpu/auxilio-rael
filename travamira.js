class MiraEstavel {
  constructor({ suavizacao = 0.2, zonaMorta = 1.5 } = {}) {
    this.s = suavizacao; // 0.05 = bem suave, 1 = sem filtro
    this.zm = zonaMorta; // pixels ignorados
    this.x = null;
    this.y = null;
  }

  atualizar(alvoX, alvoY) {
    if (this.x === null) {
      this.x = alvoX;
      this.y = alvoY;
      return { x: this.x, y: this.y };
    }
    const dx = alvoX - this.x;
    const dy = alvoY - this.y;

    // ignora tremidas pequenas
    if (Math.hypot(dx, dy) > this.zm) {
      this.x += dx * this.s;
      this.y += dy * this.s;
    }
    return { x: this.x, y: this.y };
  }
}

// Uso:
const mira = new MiraEstavel({ suavizacao: 0.15, zonaMorta: 2 });

window.addEventListener("touchmove", (e) => {
  const { x, y } = mira.atualizar(e.clientX, e.clientY);
  // desenhe a mira em (x, y)
});