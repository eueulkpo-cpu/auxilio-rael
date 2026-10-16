const switchBtn = document.getElementById('auxilio , travamira');

switchBtn.addEventListener('change', function () {
    if (this.checked) {
        minhaFuncao();
    } else {
        desativarFuncao();
    }
});

function minhaFuncao() {
    console.log('Função ativada');
}

function desativarFuncao() {
    console.log('Função desativada');
}