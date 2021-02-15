<template>
    <div>
        <app-picture-drawer />
        <app-image-upload />
        <app-image-gallery />
    </div>
</template>

<script>
import { defineComponent } from '/@utils/modifiers';
import AppPictureDrawer from '/@components/pictureDrawer.vue';
import AppImageUpload from '/@components/imageUpload.vue';
import AppImageGallery from '/@components/imageGallery.vue';

export default {
    name: 'app-picture',
    components: {
        AppPictureDrawer,
        AppImageUpload,
        AppImageGallery
    }
}
</script>
