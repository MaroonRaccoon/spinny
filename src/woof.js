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
    let posBuf = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, posBuf);
    let positions = [
        -0.25, -0.25,
        0, 0.4,
        0.2, -0.1
    ];
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

    gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);

    gl.clearColor(0, 0, 0, 0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.useProgram(program);
    gl.enableVertexAttribArray(posAttrLoc);
    gl.bindBuffer(gl.ARRAY_BUFFER, posBuf); // maybe not needed

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

