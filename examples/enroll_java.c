/*
 * Example fingerprint enrollment program
 * Enrolls your right index finger and saves the print to disk
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
#include <PRBEFS_PRBEnroll.h>
#include <jni.h>


//struct fp_print_data *enroll(struct fp_dev *dev)
JNIEXPORT jint JNICALL Java_PRBEFS_PRBEnroll_enroll
  (JNIEnv *env, jobject obj, jint finger, jstring p, jstring u)
//int enroll(int finger, char *path, char *user)
{
	struct fp_print_data *enrolled_print = NULL;

	struct fp_dscv_dev *ddev;
	struct fp_dscv_dev **discovered_devs;
	struct fp_dev *dev;
	struct fp_print_data *data;
	struct fp_driver *drv;

    const char *path = (*env)->GetStringUTFChars(env, p, 0);
    const char *user = (*env)->GetStringUTFChars(env, u, 0);

	int r;

	r = fp_init();
	fp_set_debug(3);

	if (r < 0) {
		fprintf(stderr, "Failed to initialize libfprint\n");
		exit(1);
	}

	discovered_devs = fp_discover_devs();

	if (!discovered_devs) {
		fprintf(stderr, "Could not discover devices\n");
		goto out;
	}

	ddev = discovered_devs[0];
	if (!ddev)
		return 0;

	drv = fp_dscv_dev_get_driver(ddev);

	if (!ddev) {
		fprintf(stderr, "No devices detected.\n");
		goto out;
	}

	dev = fp_dev_open(ddev);
	fp_dscv_devs_free(discovered_devs);
	if (!dev) {
		fprintf(stderr, "Could not open device.\n");
		goto out;
	}

	do {
		struct fp_img *img = NULL;

		r = fp_enroll_finger_img(dev, &enrolled_print, &img);
		if (img) {
			fp_img_save_to_file(img, "enrolled.pgm");
			//printf("Wrote scanned image to enrolled.pgm\n");
			fp_img_free(img);
		}
		if (r < 0) {
			//printf("Enroll failed with error %d\n", r);
			return 0;
		}

		/*
		switch (r) {
		case FP_ENROLL_COMPLETE:
			printf("Enroll complete!\n");
			break;
		case FP_ENROLL_FAIL:
			printf("Enroll failed, something wen't wrong :(\n");
			return NULL;
		case FP_ENROLL_PASS:
			printf("Enroll stage passed. Yay!\n");
			break;
		case FP_ENROLL_RETRY:
			printf("Didn't quite catch that. Please try again.\n");
			break;
		case FP_ENROLL_RETRY_TOO_SHORT:
			printf("Your swipe was too short, please try again.\n");
			break;
		case FP_ENROLL_RETRY_CENTER_FINGER:
			printf
			    ("Didn't catch that, please center your finger on the "
			     "sensor and try again.\n");
			break;
		case FP_ENROLL_RETRY_REMOVE_FINGER:
			printf
			    ("Scan failed, please remove your finger and then try "
			     "again.\n");
			break;
		}
		*/
	}
	while (r != FP_ENROLL_COMPLETE);

	if (!enrolled_print) {
		fprintf(stderr, "Enroll complete but no print?\n");
        fp_dev_close(dev);
		return 0;
	}

	//printf("Enrollment completed!\n\n");
	r = fp_print_data_save(enrolled_print, finger, path, user);
	if (r < 0)
		fprintf(stderr, "Data save failed, code %d\n", r);

	fp_print_data_free(enrolled_print);
 out_close:
	fp_dev_close(dev);
 out:
	fp_exit();

    (*env)->ReleaseStringUTFChars(env, p, path);
    (*env)->ReleaseStringUTFChars(env, u, user);

	return r;
}

