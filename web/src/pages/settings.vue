<template>
    <div>
        <h4 class="ui horizontal divider header">
            <i class="cogs icon"></i>
            Settings
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10">
                    <h6 class="ui horizontal divider header">
                        Upload soft
                    </h6>
                    <label class="ui right labeled icon button primary">
                        <i class="upload icon"></i>
                        Upload update
                        <input
                            type="file"
                            ref="uploader"
                            hidden
                            multiple
                            @change="upload"
                        />
                    </label >
                </div>
                <div class="mb-10">
                    <h6 class="ui horizontal divider header">
                        Refreshing time
                    </h6>
                    <app-range
                        :min="1"
                        :max="25"
                        v-model="refreshingTime"
                        @change="setRefreshingTime"
                    ></app-range>
                </div>
                <div class="mb-10">
                    <h6 class="ui horizontal divider header">
                        Colors corrections
                    </h6>
                    Red
                    <app-range
                        :min="1"
                        :max="225"
                        :modelValue="colorsCorrections[0]"
                        class="mb-10"
                        @update:modelValue="setCorrections(0, $event)"
                    ></app-range>
                    Green
                    <app-range
                        :min="1"
                        :max="225"
                        :modelValue="colorsCorrections[1]"
                        class="mb-10"
                        @update:modelValue="setCorrections(1, $event)"
                    ></app-range>
                    Blue
                    <app-range
                        :min="1"
                        :max="225"
                        :modelValue="colorsCorrections[2]"
                        class="mb-10"
                        @update:modelValue="setCorrections(3, $event)"
                    ></app-range>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '/@utils/services';
import AppRange from '/@components/range.vue';

export default {
    name: 'app-settings',
    inject: ['store'],
    components: {
        AppRange,
    },
    computed: {
        refreshingTime: {
            get() {
                return this.store.state.refreshingTime / 1000;
            },
            set(value) {
                this.store.setRefreshingTime(value * 1000);
            }
        },
        colorsCorrections() {
            return this.store.state.matrixParams.colorsCorrections || [100,100,100];
        }
    },
    methods: {
        upload (event) {
            const [file] = event.target.files;

            const formData = new FormData();
            formData.append('update', file);

            services.sendFile(formData);
        },
        setRefreshingTime(){
            services.setRefreshingTime(this.refreshingTime);
        },
        setCorrections(color, value) {
            const corrections = [...this.colorsCorrections];
            corrections[color] = parseInt(value);

            this.store.setCorrections(corrections);

            services.setPictureEffects({
                colorsCorrections: corrections,
            });
        }
    },
}
</script>
