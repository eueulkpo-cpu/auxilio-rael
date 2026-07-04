function createTouchHook({ dragReduction = 0.4, speedMultiplier = 0.6, maxAimSpeed = 300 } = {}) {
 let origin = null;
 let hookAtivo = false;

 function getRawTouch(e) {
   const t = e.touches ? e.touches[0] : e;
   const now = Date.now();
   const dt = origin ? (now - origin.time) / 1000 || 0.001 : 0.001;
   const dx = origin ? t.clientX - origin.x : 0;
   const dy = origin ? t.clientY - origin.y : 0;
   const speed = Math.sqrt(dx * dx + dy * dy) / dt;

   return { x: t.clientX, y: t.clientY, dx, dy, speed };
 }

 function modifyInput(raw) {
   const brake  = raw.speed > maxAimSpeed ? maxAimSpeed / raw.speed : 1;
   const factor = (1 - dragReduction) * brake;

   return {
     x:     raw.x,
     y:     raw.y,
     dx:    raw.dx    * factor,
     dy:    raw.dy    * factor,
     speed: raw.speed * speedMultiplier * brake,
   };
 }

 function sendToGame(modified) {
   console.log("🎮 Input enviado ao jogo:", modified);
    aim.x += modified.dx;
    aim.y += modified.dy;
 }

 function onTouchStart(e) {
   const t = e.touches ? e.touches[0] : e;
   origin = { x: t.clientX, y: t.clientY, time: Date.now() };
   console.log("🖐 RAW capturado:", origin);
 }

 function onTouchMove(e) {
   if (!origin) return;
   const raw      = getRawTouch(e);
   const modified = modifyInput(raw);
   sendToGame(modified);
   const t = e.touches ? e.touches[0] : e;
   origin = { x: t.clientX, y: t.clientY, time: Date.now() };
 }

 function onTouchEnd() {
   origin = null;
   console.log("✅ Toque finalizado");
 }

 // ── Ativar / Desativar ──
 function ativar() {
   if (hookAtivo) return;

   document.addEventListener("mousedown",  onTouchStart);
   document.addEventListener("mousemove",  onTouchMove);
   document.addEventListener("mouseup",    onTouchEnd);
   document.addEventListener("touchstart", onTouchStart);
   document.addEventListener("touchmove",  onTouchMove);
   document.addEventListener("touchend",   onTouchEnd);

   hookAtivo = true;
   console.log("✅ Hook ativado");
 }

 function desativar() {
   if (!hookAtivo) return;

   document.removeEventListener("mousedown",  onTouchStart);
   document.removeEventListener("mousemove",  onTouchMove);
   document.removeEventListener("mouseup",    onTouchEnd);
   document.removeEventListener("touchstart", onTouchStart);
   document.removeEventListener("touchmove",  onTouchMove);
   document.removeEventListener("touchend",   onTouchEnd);

   hookAtivo = false;
   console.log("❌ Hook desativado");
 }

 return { ativar, desativar };
}

// ── Uso ──
const hook = createTouchHook({
 dragReduction:   0.4,
 speedMultiplier: 0.6,
 maxAimSpeed:     300,
});

document.getElementById("btnAtivar").addEventListener("click", hook.ativar);
document.getElementById("btnDesativar").addEventListener("click", hook.desativar);
