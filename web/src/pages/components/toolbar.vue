<template>
    <div class="toolbar">
        <div class="ui  center" style="width: auto;">
            <div class="content">

                <div class="ui small basic icon buttons">
                    <button
                        class="ui circular facebook icon button"
                        @click="rotate"
                    >
                        <i class="retweet icon"></i>
                        <span>Rotate</span>
                    </button>
                    <button
                        class="ui circular facebook icon button"
                        @click="refresh"
                    >
                        <i class="sync icon"></i>
                        <span>Refresh</span>
                    </button>
                    <button
                        class="ui circular facebook icon button"
                        @click="toggleSpeed"
                    >
                        <i class="angle double right icon"></i>
                        <span>Speed</span>
                    </button>
                    <button
                        class="ui circular facebook icon button"
                        @click="toggleBrightness"
                    >
                        <i class="sun outline icon"></i>
                        <span>Brightness</span>
                    </button>
                </div>

                <div v-show="isSpeedShown" class="speed-range">
                    <i class="angle double right icon"></i>
                    <app-range
                        style="width: 100%"
                        :min="1"
                        :max="store.state.matrixParams.speedMax"
                        v-model="speed"
                    ></app-range>
                </div>

                <div v-show="isBrightnessShown" class="speed-range">
                    <i class="sun outline right icon"></i>
                    <app-range
                        style="width: 100%"
                        :min="1"
                        :max="255"
                        v-model="brightness"
                    ></app-range>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import AppRange from '/@components/range.vue';
import services from "/@utils/services";

export default {
    name: 'app-toolbar',
    inject: ['store'],
    components: {
        AppRange,
    },
    data() {
        return {
            isSpeedShown: false,
            isBrightnessShown: false,
            speed: this.store.state.matrixParams.speed,
            brightness: this.store.state.matrixParams.brightness,
            second: 1000
        };
    },
    watch:{
        speed() {
            this.setSpeed()
        },
        brightness() {
            this.setBrightness()
        }
    },
    methods: {
        toggleSpeed() {
          this.isSpeedShown = !this.isSpeedShown;
        },
        toggleBrightness() {
            this.isBrightnessShown = !this.isBrightnessShown;
        },
        setSpeed() {
            services.setSpeed(this.speed).then(() => {
                // this.isSpeedShown = false;
            });
        },
        setBrightness() {
            services.setBrightness(this.brightness).then(() => {
                // this.isBrightnessShown = false;
            });
        },
        rotate() {
            services.rotate();
        },
        refresh() {
            services.getMatrixState();
        },
    },
}
</script>

<style scoped>
    .toolbar .buttons {
        display: flex;
        justify-content: flex-end;
    }

    .speed-range {
        display: flex;
        padding: 20px 0;
    }

    .buttons .button span {
        display: block;
        font-size: 10px;
    }
</style>
