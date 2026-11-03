const area = document.getElementById("area");
const ponto = document.getElementById("ponto");

let centroY = 150;
let ativo = false;

area.addEventListener("pointerdown", (e) => {
  ativo = true;
  area.setPointerCapture(e.pointerId);
});

area.addEventListener("pointermove", (e) => {
  if (!ativo) return;

  const rect = area.getBoundingClientRect();

  let x = e.clientX - rect.left;
  let y = e.clientY - rect.top;

  // Limita o movimento para cima
  y = Math.max(centroY, y);

  // Limita os movimentos dentro da área
  x = Math.max(17, Math.min(283, x));
  y = Math.min(283, y);

  ponto.style.left = (x - 17) + "px";
  ponto.style.top = (y - 17) + "px";
});

area.addEventListener("pointerup", () => {
  ativo = false;
});