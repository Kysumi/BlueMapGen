const temp = () => {
    console.log("Demo Arrow Function");
};

console.log(12312312312);

temp();

let thenable = {
    then(a, b) {
        console.log('then called');
        a(1);
    },
};

let native = new Promise(resolve => resolve(1));

console.log(0);

Promise.resolve(thenable).then(() => {
    console.log(1);
});

Promise.resolve(native).then(() => {
    console.log(2);
});

console.log('asdasdasdasdasas');