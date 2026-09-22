let toqueX = 0;
let toqueY = 0;

let x = 0;
let y = 0;

document.addEventListener("touchstart", function (event) {
    const touch = event.touches[0];

    toqueX = toque.clientX;
    toqueY = toque.clientY;
});

function atualizarMira() {
    x = toqueX;
    y = toqueY;

    mira.style.transform = `translate3d(${x}px, ${y}px ${0}px)`;

    requestAnimationFrame(atualizarMira);
}

requestAnimationFrame(atualizarMira);

function mira(){    
    let miraX = 10;
    let miraY = 20;
}