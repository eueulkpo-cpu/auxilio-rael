let toqueX = 0;
let toque = 0;

let x = 0;
let y = 0;

document.addEventListener("touchstart", function (event) {
    const touch = event.touches[0];

    toqueX = toque.clientX;
    toque = toque.clientX;
});

function atualizarMira() {
    x = toqueX;
    y = toque;

    mira.style.transform = `translate3d(${x}px, ${y}px ${0}px)`;

    requestAnimationFrame(atualizarMira);
}

requestAnimationFrame(atualizarMira);

