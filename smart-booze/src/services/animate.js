import anime from 'animejs';

export function translate(element, deg) {
    console.log("click");
    // element.style.top = -15 + "px";
    // element.style.left = 15 + "px";
    anime({
        targets: element,
        rotate: deg,
    });
}