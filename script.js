const checkbox = document.getElementById('performance');

checkbox.addEventListener('change', () => {
    engine.active = checkbox.checked;

    if (checkbox.checked) {
        console.log('Performance mode activated');
    } else {
        console.log('Performance mode deactivated');
    }
});
    
