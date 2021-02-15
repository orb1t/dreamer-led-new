<template>
    <div>
        <h4 class="ui horizontal divider header">
            <i class="keyboard outline icon"></i>
            Running text
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10">
                    <select
                        v-model="selectedEffect"
                        ref="dropdown"
                        class="ui dropdown"
                    >
                        <option value="">Select effect</option>
                        <option
                            v-for="(value, label) in effectsList"
                            :key="label"
                            :value="value"
                        >{{ label }}</option>
                    </select>
                </div>
            </div>
        </div>
        <h4 class="ui horizontal divider header">
            <i class="cogs icon"></i>
            Settings
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content center">
                <button
                    class="ui right labeled icon button primary mb-10"
                    @click="pickerMode = pickerModes.color; picker.show();"
                >
                    <i class="eye dropper icon"></i> Set color
                </button>
                <i
                    :style="'color: rgb(' + srcColor[0] + ',' + srcColor[1] + ',' + srcColor[2] + ');'"
                    class="circle icon"
                ></i>

                <button
                    class="ui right labeled icon button teal mb-10"
                    @click="pickerMode = pickerModes.backgroundColor; picker.show();"
                >
                    <i class="magic icon"></i> Set background color
                </button>
                <i
                    :style="'color: rgb(' + srcBackgroundColor[0] + ',' + srcBackgroundColor[1] + ',' + srcBackgroundColor[2] + ');'"
                    class="circle icon"
                ></i>

                <div ref="colorPicker"></div>

            </div>
            <div class="content center" v-show="selectedEffect === effectsList.text">
                <div class="mb-10">
                    <div class="ui input mb-10" style="margin-right: 20px;">
                        <input
                            v-model="textConfig.string"
                            type="text"
                            placeholder="Some text example"
                        >
                    </div>
                    <div class="ui toggle checkbox">
                        <input v-model="textConfig.active" type="checkbox" name="gift" tabindex="0" class="hidden">
                        <label>Is active</label>
                    </div>
                </div>
                <div class="mb-10">
                    <app-range
                        :min="1"
                        :max="255"
                        v-model="textConfig.speed"
                    ></app-range>
                </div>
            </div>
            <div class="content form" v-show="selectedEffect === effectsList.clock">
                <div class="grouped fields center">
                    <label for="fruit">Syncronize clock:</label>
                    <div class="field">
                        <div class="ui radio checkbox">
                            <input
                                v-model="clockConfig.src"
                                value="0"
                                type="radio" name="fruit" checked="" tabindex="0" class="hidden"
                            >
                            <label>From internet</label>
                        </div>
                    </div>
                    <div class="field">
                        <div class="ui radio checkbox">
                            <input
                                v-model="clockConfig.src"
                                value="1"
                                type="radio" name="fruit" tabindex="0" class="hidden"
                            >
                            <label>From phone</label>
                        </div>
                    </div>
                </div>
            </div>
            <div class="content">
                <div class="center">
                    <button
                        class="ui right labeled icon button green"
                        @click="apply"
                    >
                        <i class="check icon"></i>
                        Apply
                    </button>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '/@utils/services';
import Picker from 'vanilla-picker';
import AppRange from '/@components/range.vue';

export default {
    name: 'app-text',
    components: {
        AppRange
    },
    data() {
        return {
            pickerMode: 0,
            pickerModes: {
                color: 0,
                backgroundColor: 1,
            },
            selectedEffect: 0,
            srcColor: [255,255,255,1],
            currentColor: 'rgba(255,255,255,1)',
            srcBackgroundColor: [0,0,0,1],
            currentBackgroundColor: 'rgba(0,0,0,,1)',
            effectsList: {
                text: 0,
                clock: 1,
            },
            textConfig: {
                string: '',
                speed: 0,
                active: false
            },
            clockConfig: {
                src: 0,
            },
        };
    },
    computed: {
        color() {
            const [r, g, b] = this.srcColor;
            return {r, g, b};
        },
        backgroundColor() {
            const [r, g, b] = this.srcBackgroundColor;
            return {r, g, b};
        }
    },
    methods: {
        initPicker() {
            const pickerEl = this.$refs.colorPicker;
            this.picker = new Picker(pickerEl);
            this.picker.setOptions({
                popup: 'bottom',
            });
            
            this.picker.onDone = color =>  {
                if (this.pickerMode === this.pickerModes.color) {
                    this.srcColor = color.rgba;
                    this.currentColor = color.rgbaString;
                }
                else if (this.pickerMode === this.pickerModes.backgroundColor) {
                    this.srcBackgroundColor = color.rgba;
                    this.currentBackgroundColor = color.rgbaString;
                }
            };
        },
        apply() {
            if (this.selectedEffect == this.effectsList.text) {
                services.setRunningText({
                    ...this.textConfig,
                    color: this.color,
                    backgroundColor: this.backgroundColor
                });
            }
            else if (this.selectedEffect == this.effectsList.clock) {
                services.setClock({
                    src: +this.clockConfig.src,
                    phoneTime: new Date().getTime(),
                    color: this.color,
                    backgroundColor: this.backgroundColor
                });
            }
        },
    },
    mounted() {
        $('.ui.dropdown').dropdown();
        $('.ui.radio.checkbox').checkbox();
        $('.ui.checkbox').checkbox();

        this.initPicker();
    }
}
</script>
