/*
 * Example fingerprint verification program, which verifies the right index
 * finger which has been previously enrolled to disk.
 * Copyright (C) 2007 Daniel Drake <dsd@gentoo.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libfprint/fprint.h>
#include <PRBAS_PRBAttendance.h>
#include <jni.h>

JNIEXPORT jint JNICALL Java_PRBAS_PRBAttendance_verify
  (JNIEnv *env, jobject obj, jint finger, jstring p, jstring u)
{
	int r=0;
    int status;

	struct fp_img *img = NULL;
	struct fp_dscv_dev *ddev;
	struct fp_dscv_dev **discovered_devs;
	struct fp_dev *dev;
	struct fp_print_data *data;
    struct fp_driver *drv;

    jboolean iscopy1;
    jboolean iscopy2;

    char *path = (*env)->GetStringUTFChars(env, p, &iscopy1);
    char *user = (*env)->GetStringUTFChars(env, u, &iscopy2);

	r = fp_init();
	if (r < 0) {
		fprintf(stderr, "Failed to initialize libfprint\n");
        status = 0;
        goto out;
	}

	fp_set_debug(3);

	discovered_devs = fp_discover_devs();
	if (!discovered_devs) {
		fprintf(stderr, "Could not discover devices\n");
        status = 0;
		goto out;
	}

    ddev = discovered_devs[0];
    if (!ddev) {
		fprintf(stderr, "Could not discover devices\n");
        status = 0;
        goto out;
    }

    drv = fp_dscv_dev_get_driver(ddev);

    if (!ddev) {
        fprintf(stderr, "No devices detected.\n");
        status = 0;
        goto out;
    }

    dev = fp_dev_open(ddev);
    fp_dscv_devs_free(discovered_devs);
    if (!dev) {
        fprintf(stderr, "Could not open device.\n");
        status = 0;
        goto out;
    }

	r = fp_print_data_load(dev, finger, &data, path, user);

    printf("path = %s, user = %s\n", path, user);

    if(iscopy1 == JNI_TRUE) 
        (*env)->ReleaseStringUTFChars(env, p, path);

    if(iscopy2 == JNI_TRUE) 
        (*env)->ReleaseStringUTFChars(env, u, user);

	if (r != 0) {
		fprintf(stderr, "Failed to load fingerprint, error %d\n", r);
		fprintf(stderr, "Did you remember to enroll your finger "
			"first?\n");
        status = 0;
		goto out_close;
	}

    r = fp_verify_finger_img(dev, data, &img);
    if (img) {
        fp_img_save_to_file(img, "verify.pgm");
        fp_img_free(img);
    }


	if (r < 0) {
		fprintf(stderr, "verification failed with error %d :(\n", r);
        status = 0;
	}

	switch (r) {
	    case FP_VERIFY_NO_MATCH:
		    //printf("NO MATCH!\n");
			status = 0;
            break;
		case FP_VERIFY_MATCH:
			//printf("MATCH!\n");
            status = 1;
            break;
		case FP_VERIFY_RETRY:
			//printf("Scan didn't quite work. Please try again.\n");
            status = -1;
            break;
		case FP_VERIFY_RETRY_TOO_SHORT:
			//printf("Swipe was too short, please try again.\n");
            status = -1;
            break;
		case FP_VERIFY_RETRY_CENTER_FINGER:
			//printf("Please center your finger on the sensor and try again.\n");
            status = -1;
            break;
		case FP_VERIFY_RETRY_REMOVE_FINGER:
			//printf("Please remove finger from the sensor and try again.\n");
            status = -1;
            break;
	}
out_close:
	fp_print_data_free(data);
	fp_dev_close(dev);
out:
	fp_exit();


    return status;
}


