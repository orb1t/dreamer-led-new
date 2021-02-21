<template>
    <div>
        <template v-if="!disabled">
            <h4 class="ui horizontal divider header">
                <i class="bar pencil icon"></i>
                Draw a picture
            </h4>
            <div class="ui card" style="width: auto;">
                <div class="content">
                    <div ref="wrapper">
                        <div class="upload__button">
                            <button
                                class="ui right labeled icon button green"
                                @click="isFillMode = true; picker.show();"
                            >
                                <i class="eye dropper icon"></i>
                                Fill field
                            </button>
                        </div>
                    </div>
                </div>
            </div>
        </template>
        <div
            class="ui card"
            style="width: auto;"
            :style="disabled ? 'pointer-events: none;' : ''"
        >
            <div class="content">
                <div ref="colorPicker"></div>
                <div
                    ref="drawer"
                    class="drawer"
                    @touchstart="startDraw"
                    @touchmove="doDraw"
                    @touchend="endDraw"
                    @mousedown="startDraw"
                    @mousemove="doDraw"
                    @mouseup="endDraw"
                    @contextmenu.prevent="picker.show()"
                >
                    <div
                        v-for="(row, i) in matrix"
                        :key="row"
                        :style="'width: ' + charSize * matrixParams.width + 'px;'"
                        class="drawer__row"
                    >
                        <div
                            v-for="(col, j) in row"
                            :key="row + j"
                            class="drawer__col"
                            :style="'width: ' + charSize + 'px; height: ' + charSize + 'px; background: ' + col.background + ';'"
                            :data-x="j"
                            :data-y="i"
                        ></div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '/@utils/services';
import Picker from 'vanilla-picker';

export default {
    name: 'app-picture-drawer',
    inject: ['store'],
    props: {
        disabled: Boolean,
    },
    data() {
        return {
            currentColor: 'rgb(0, 0, 0, 0)',
            srcColor: [0, 0, 0, 0],
            matrix: [
                [
                    {
                        background: 'rgb(0,0,0)',
                    },
                ],
            ],
            isDrawing: false,
            prevDrawnElement: null,
            prevTouchedElement: null,
            charSize: 0,
            picker: null,
            isFillMode: false,
        };
    },
    computed: {
        matrixParams() {
            return this.store.state.matrixParams;
        },
        timeDelay() {
            return this.store.state.refreshingTime;
        }
    },
    watch: {
        matrixParams: {
            deep: true,
            handler() {
                this.calculateCharSize();
            },
        },
        'store.state.isImageLoaded'(is) {
            this.refresh();
        },
        'store.state.matrixContent'(value) {
            // if user is currently drawing than
            // drawer shouldn't be refreshed from server
            if (this.isDrawing || !this.store.state.matrixContent) return;
            this.redrawMatrix(value);
        }
    },
    methods: {
        calculateCharSize() {
            const drawer = this.$refs.drawer;
            if (!drawer) return 0;

            const { width } = drawer.getBoundingClientRect();

            this.charSize = width / this.matrixParams.width;
        },
        initPicker() {
            const pickerEl = this.$refs.colorPicker;
            this.picker = new Picker(pickerEl);
            this.picker.setOptions({
                popup: 'bottom',
            });

            this.picker.onDone = color =>  {
                this.srcColor = color.rgba;
                this.currentColor = color.rgbaString;
                if (this.isFillMode) {
                    this.fillDrawer();
                  // this.isFillMode = false;
                }
            };
        },
        callPicker(event) {
            const xcoord = event.touches? event.touches[0].clientX : event.clientX;
            const ycoord = event.touches? event.touches[0].clientY : event.clientY;

            const targetElement = document.elementFromPoint(xcoord, ycoord);
            this.prevTouchedElement = targetElement;

            setTimeout(() => {
                if (targetElement === this.prevTouchedElement) {
                    this.picker.show();
                }
            }, 1000);
        },
        startDraw(event) {
            this.isDrawing = true;
            this.doDraw(event);
            this.callPicker(event);
        },
        doDraw(event) {
            event.preventDefault();
            if (!this.isDrawing) return;

            const xcoord = event.touches? event.touches[0].clientX : event.clientX;
            const ycoord = event.touches? event.touches[0].clientY : event.clientY;

            const targetElement = document.elementFromPoint(xcoord, ycoord);

            if (this.prevDrawnElement === targetElement) return;
            this.prevDrawnElement = targetElement;
            this.prevTouchedElement = targetElement;

            if (targetElement && targetElement.classList.contains('drawer__col')) {
                const { x, y } = targetElement.dataset;
                this.matrix[y][x].background = this.currentColor;

                const [r, g, b] = this.srcColor;
                this.sendData({ x: +x, y: +y, r, g, b });
            }

        },
        endDraw() {
            this.isDrawing = false;
        },
        initMatrix() {
            const widthList = Array(this.matrixParams.width).fill().map((i, j) => j);
            const heightList = Array(this.matrixParams.height).fill().map((i, j) => j);

            this.matrix = heightList.map(i => widthList.map(j => {
                    const index = i * widthList.length + j ;

                    return {
                        background: 'rgb(0,0,0)',
                    }
                })
            );
        },
        redrawMatrix(values) {
          let index = 0;
        //   console.log(`MATRIX: ${JSON.stringify(this.matrix)}`);
          let _w = this.matrix.length;
          let _h = this.matrix[1].length;
        //   console.log(`width: ${_w}, height: ${_h}`);
          let _idx = 0;
          this.matrix.forEach((row, i) => {
              row.forEach((col, j) => {
                let odd = i % 2 != 0;
                _idx = ( ( odd ? i + 1 : i ) * 3 * _w + ( odd ? -3 : 0 ) ) + ( ( odd ? -j : j ) * 3 );
                // console.log(`i: ${i}, j: ${j}, index: ${index}, _idx: ${_idx}`);
                let r = values[_idx++];
                let g = values[_idx++];
                let b = values[_idx++];
                // const r = values[index++];
                // const g = values[index++];
                // const b = values[index++];
                col.background = `rgb(${r},${g},${b})`;
              })
          })
          let _y;
          let _x;
          for ( _y = 0; _y < _h; _y++ )
            for ( _x = 0; _x <= _w / 2; _x++ ){
              let c = this.matrix[_x,_y].background;
              this.matrix[_x,_y].background = this.matrix[_w-_x,_y].background;
              this.matrix[_w-_x,_y].background = c;
            }
        },
        sendData(params) {
            services.drawSinglePixel(params);
        },
        refresh() {
            services.getMatrixState();
        },
        fillDrawer() {
            const [r, g, b] = this.srcColor;
            services.fillMatrix({ r, g, b });
        }
    },
    mounted() {
        this.initMatrix();
        this.initPicker();
        this.calculateCharSize();
        this.refresh();
    },
}
</script>
