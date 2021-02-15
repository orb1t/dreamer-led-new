export default {
    methods: {
        checkFile(file) {
            if (file.type !== '' && !file.type.match('image.*')) return;

            window.URL = window.URL || window.webkitURL;

            return window.URL.createObjectURL(file);
        },
        getPixel (imageData, x, y) {
            const index = (x + y * imageData.width) * 4;
            return {
                r: imageData.data[index],
                g: imageData.data[index + 1],
                b: imageData.data[index + 2],
                a: imageData.data[index + 3],
            };
        },
    },
    computed: {
        OUT_SRC() {
            const widthList = Array(this.OUT_WIDTH).fill().map((i, j) => j);
            const heightList = Array(this.OUT_HEIGTH).fill().map((i, j) => j);

            const byteArray = new Uint8Array(this.OUT_HEIGTH * this.OUT_WIDTH * 3);

            heightList.forEach(y => {
                widthList.forEach(x => {
                    let index = (y * widthList.length + x) * 3;
                    const {r, g, b} = this.getPixel(this.imgData, x, y);
                    byteArray[index++] = r;
                    byteArray[index++] = g;
                    byteArray[index++] = b;
                });
            });

            return byteArray;
        },
    },
}
