window.onload = main;

function main() {
    let canvas = document.querySelector("#canvas");
    let gl = canvas.getContext("webgl");
    if (!gl) {
        console.error("webgl not found :(");
    }

    let shaderSource = {
        vert: document.querySelector("#vert-shader").text,
        frag: document.querySelector("#frag-shader").text,
    };
    let shader = {
        vert: createShader(gl, gl.VERTEX_SHADER, shaderSource.vert),
        frag: createShader(gl, gl.FRAGMENT_SHADER, shaderSource.frag),
    };
    let program = createProgram(gl, shader.vert, shader.frag);

    let posAttrLoc = gl.getAttribLocation(program, "position");

    resizeCanvasToDisplaySize(gl.canvas);
    gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);

    gl.clearColor(0, 0, 0, 0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.useProgram(program);
    gl.enableVertexAttribArray(posAttrLoc);

    let posBuf1 = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, posBuf1);
    let positions = [
        -0.25, -0.25,
        0, 0.4,
        0.2, -0.1
    ];
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

    let posBuf2 = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, posBuf2);
    positions = [
        0.5, 0.5,
        0.5, 0.75,
        0.7, 0.6
    ];
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

    gl.bindBuffer(gl.ARRAY_BUFFER, posBuf1);
    {
        let attr = {
            size: 2,
            type: gl.FLOAT,
            normalize: false,
            stride: 0,
            offset: 0,
        };
        gl.vertexAttribPointer(posAttrLoc, attr.size, attr.type, attr.normalize, attr.stride, attr.offset);
    }

    {
        let attr = {
            primitiveType: gl.TRIANGLES,
            offset: 0,
            count: 3,
        };
        gl.drawArrays(attr.primitiveType, attr.offset, attr.count);
    }

    gl.bindBuffer(gl.ARRAY_BUFFER, posBuf2);
    {
        let attr = {
            size: 2,
            type: gl.FLOAT,
            normalize: false,
            stride: 0,
            offset: 0,
        };
        gl.vertexAttribPointer(posAttrLoc, attr.size, attr.type, attr.normalize, attr.stride, attr.offset);
    }

    {
        let attr = {
            primitiveType: gl.TRIANGLES,
            offset: 0,
            count: 3,
        };
        gl.drawArrays(attr.primitiveType, attr.offset, attr.count);
    }
}

function loop(gl) {
}


function createShader(gl, type, source) {
    let shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);
    let success = gl.getShaderParameter(shader, gl.COMPILE_STATUS);
    if (success) {
        return shader;
    }
    console.log(gl.getShaderInfoLog(shader));
    gl.deleteShader(shader);
}

function createProgram(gl, vertexShader, fragmentShader) {
    let program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);
    let success = gl.getProgramParameter(program, gl.LINK_STATUS);
    if (success) {
        return program;
    }

    console.log(gl.getProgramInfoLog(program));
    gl.deleteProgram(program);
}


function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

function resizeCanvasToDisplaySize(canvas) {
    // Lookup the size the browser is displaying the canvas in CSS pixels.
    const displayWidth = canvas.clientWidth;
    const displayHeight = canvas.clientHeight;
    console.log(`displayWidth: ${displayWidth}`);
    console.log(`displayHeight: ${displayHeight}`);
    console.log(`canvas.width: ${canvas.width}`);
    console.log(`canvas.height: ${canvas.height}`);

    // Check if the canvas is not the same size.
    const needResize = canvas.width !== displayWidth ||
        canvas.height !== displayHeight;
    console.log(`needResize: ${needResize}`);

    if (needResize) {
        // Make the canvas the same size
        canvas.width = displayWidth;
        canvas.height = displayHeight;
    }
    console.log(`canvas.width: ${canvas.width}`);
    console.log(`canvas.height: ${canvas.height}`);

    return needResize;
}
