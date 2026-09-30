let mode = "smooth"; // Default mode

let currentX = 0;
let currentY = 0;

function setMode(newMode) {
    mode = newMode;
}

function smoothMovement(targetX, targetY) {
    let factor;
    switch (mode) {

        case "smooth":
            factor = 0.12; 
            break;

        case "tabilizer":
            factor = 0.07; 
            break;

        case "precision":
            factor = 0.04
            break;

        default:
            factor = 0.12;  
    }

    currentX += (targetX - currentX) * factor;
    currentY += (targetY - currentY) * factor;

    return { x: currentX, y: currentY };
  };
  
//exemplos
setMode("smooth");
console.log(smoothMovement(100, 50)); // Smooth movement

setMode("tabilizer");
console.log(smoothMovement(150, 80)); // Stabilizer movement

setMode("precision");
console.log(smoothMovement(200, 100)); // Precision movement
