/**
 * Copyright 2010-2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * <p>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 * <p>
 * http://aws.amazon.com/apache2.0
 * <p>
 * This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
 * OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and
 * limitations under the License.
 */

package com.amazonaws.demo.androidpubsub;

import android.app.Activity;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.amazonaws.auth.CognitoCachingCredentialsProvider;
import com.amazonaws.mobile.client.AWSMobileClient;
import com.amazonaws.mobile.client.Callback;
import com.amazonaws.mobile.client.UserStateDetails;
import com.amazonaws.mobileconnectors.iot.AWSIotKeystoreHelper;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttLastWillAndTestament;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttManager;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttNewMessageCallback;
import com.amazonaws.mobileconnectors.iot.AWSIotMqttQos;
import com.amazonaws.regions.Region;
import com.amazonaws.regions.Regions;
import com.amazonaws.services.iot.AWSIotClient;
import com.amazonaws.services.iot.model.AttachPrincipalPolicyRequest;
import com.amazonaws.services.iot.model.CreateKeysAndCertificateRequest;
import com.amazonaws.services.iot.model.CreateKeysAndCertificateResult;

import java.io.UnsupportedEncodingException;
import java.security.KeyStore;
import java.util.UUID;

import static android.os.SystemClock.sleep;

public class PubSubActivity extends Activity {

    static final String LOG_TAG = PubSubActivity.class.getCanonicalName();

    // --- Constants to modify per your configuration ---

    // IoT endpoint
    // AWS Iot CLI describe-endpoint call returns: XXXXXXXXXX.iot.<region>.amazonaws.com
    // private static final String CUSTOMER_SPECIFIC_ENDPOINT = "atry2dcv6if0n-ats.iot.us-west-2.amazonaws.com";
    private static final String CUSTOMER_SPECIFIC_ENDPOINT = "atry2dcv6if0n-ats.iot.us-west-2.amazonaws.com";
    // Name of the AWS IoT policy to attach to a newly created certificate
    private static final String AWS_IOT_POLICY_NAME = "App_Policy";

    // Region of AWS IoT
    private static final Regions MY_REGION = Regions.US_WEST_2;
    // Filename of KeyStore file on the filesystem
    private static final String KEYSTORE_NAME = "iot_keystore";
    // Password for the private key in the KeyStore
    private static final String KEYSTORE_PASSWORD = "password";
    // Certificate and key aliases in the KeyStore
    private static final String CERTIFICATE_ID = "default";

    EditText txtSubcribe;
    //EditText txtTopic;
    //EditText txtMessage;

    TextView tvLastMessage;
    //TextView tvClientId;
    TextView tvStatus;
    TextView ZigbeeStatus;
    TextView GadgetStatus;
    TextView HomekitStatus;
    TextView MqttStatus;

    Button btnConnect;

    AWSIotClient mIotAndroidClient;
    AWSIotMqttManager mqttManager;
    String clientId;
    String keystorePath;
    String keystoreName;
    String keystorePassword;

    KeyStore clientKeyStore = null;
    String certificateId;

//    public void connectClick(final View view) {
//        Log.d(LOG_TAG, "clientId = " + clientId);
//
//        try {
//            mqttManager.connect(clientKeyStore, new AWSIotMqttClientStatusCallback() {
//                @Override
//                public void onStatusChanged(final AWSIotMqttClientStatus status,
//                                            final Throwable throwable) {
//                    Log.d(LOG_TAG, "Status = " + String.valueOf(status));
//
//                    runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
//                            //tvStatus.setText(status.toString());
//                            Toast.makeText(getApplicationContext(), status.toString(), Toast.LENGTH_LONG).show();
//                            if (throwable != null) {
//                                Log.e(LOG_TAG, "Connection error.", throwable);
//                            }
//                        }
//                    });
//                }
//            });
//        } catch (final Exception e) {
//            Log.e(LOG_TAG, "Connection error.", e);
//            Toast.makeText(getApplicationContext(), "Error! " + e.getMessage(), Toast.LENGTH_LONG).show();
//            //tvStatus.setText("Error! " + e.getMessage());
//        }
//    }

    public void bindClick(final View view){
        final String topic = "CheckLightStatus";
        Log.d(LOG_TAG, "topic = " + topic);
        Toast.makeText(getApplicationContext(), "Subscribe to CheckLightStatus", Toast.LENGTH_LONG).show();

        try {
            mqttManager.subscribeToTopic(topic, AWSIotMqttQos.QOS0,
                    new AWSIotMqttNewMessageCallback() {
                        @Override
                        public void onMessageArrived(final String topic, final byte[] data) {
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    try {
                                        String message = new String(data, "UTF-8");
                                        Log.d(LOG_TAG, "Message arrived:");
                                        Log.d(LOG_TAG, "   Topic: " + topic);
                                        Log.d(LOG_TAG, " Message: " + message);

                                        tvLastMessage.setText(message);
                                    } catch (UnsupportedEncodingException e) {
                                        Log.e(LOG_TAG, "Message encoding error.", e);
                                    }
                                }
                            });
                        }
                    });
        } catch (Exception e) {
            Log.e(LOG_TAG, "Subscription error.", e);
        }
    }
//    public void subscribeClick(final View view) {
//        final String topic = txtSubcribe.getText().toString();
//        final String topic = "CheckLightStatus";
//        Log.d(LOG_TAG, "topic = " + topic);
//
//        try {
//            mqttManager.subscribeToTopic(topic, AWSIotMqttQos.QOS0,
//                    new AWSIotMqttNewMessageCallback() {
//                        @Override
//                        public void onMessageArrived(final String topic, final byte[] data) {
//                            runOnUiThread(new Runnable() {
//                                @Override
//                                public void run() {
//                                    try {
//                                        String message = new String(data, "UTF-8");
//                                        Log.d(LOG_TAG, "Message arrived:");
//                                        Log.d(LOG_TAG, "   Topic: " + topic);
//                                        Log.d(LOG_TAG, " Message: " + message);
//
//                                        tvLastMessage.setText(message);
//                                    } catch (UnsupportedEncodingException e) {
//                                        Log.e(LOG_TAG, "Message encoding error.", e);
//                                    }
//                                }
//                            });
//                        }
//                    });
//        } catch (Exception e) {
//            Log.e(LOG_TAG, "Subscription error.", e);
//        }
//    }

    /*
    * Light Control("on" and "off")
     */
    public void turnonClick(final View view) {
        final String topic = "myLightBulb";
        final String msg = "t";

        try {
            mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);
            Toast.makeText(getApplicationContext(), "Turn on the light~", Toast.LENGTH_LONG).show();
        } catch (Exception e) {
            Log.e(LOG_TAG, "Publish error.", e);
        }
    }

    public void turnoffClick(final View view) {
        final String topic = "myLightBulb";
        final String msg = "f";

        try {
            mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);
            Toast.makeText(getApplicationContext(), "Turn off the light~", Toast.LENGTH_LONG).show();
        } catch (Exception e) {
            Log.e(LOG_TAG, "Publish error.", e);
        }
    }


    /*
     * Zigbee Control("enable" and "disable")
     */
    public void enableZigbeeClick(final View view) {
        final String topic = "myLightBulb";
        final String msg = "Z";

        try {
            mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);
            Toast.makeText(getApplicationContext(), "Enable Zigbee Channel~", Toast.LENGTH_LONG).show();
            Drawable drawable= getResources().getDrawable(android.R.drawable.presence_online);
            ZigbeeStatus.setCompoundDrawablesWithIntrinsicBounds(drawable,null, null, null);
            ZigbeeStatus.setCompoundDrawablePadding(4);
        } catch (Exception e) {
            Log.e(LOG_TAG, "Publish error.", e);
        }
    }

    public void disableZigbeeClick(final View view) {
        final String topic = "myLightBulb";
        final String msg = "z";

        try {
            mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);
            Toast.makeText(getApplicationContext(), "Disable Zigbee Channel~", Toast.LENGTH_LONG).show();
            Drawable drawable= getResources().getDrawable(android.R.drawable.presence_invisible);
            ZigbeeStatus.setCompoundDrawablesWithIntrinsicBounds(drawable,null, null, null);
            ZigbeeStatus.setCompoundDrawablePadding(4);
        } catch (Exception e) {
            Log.e(LOG_TAG, "Publish error.", e);
        }
    }

    /*
     * Gadget Control("enable" and "disable")
     */
    public void enableGadgetClick(final View view) {
        final String topic = "myLightBulb";
        final String msg = "G";

        try {
            mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);
            Toast.makeText(getApplicationContext(), "Enable Gadget Channel~", Toast.LENGTH_LONG).show();
            Drawable drawable= getResources().getDrawable(android.R.drawable.presence_online);
            GadgetStatus.setCompoundDrawablesWithIntrinsicBounds(drawable,null, null, null);
            GadgetStatus.setCompoundDrawablePadding(4);
        } catch (Exception e) {
            Log.e(LOG_TAG, "Publish error.", e);
        }
    }

    public void disableGadgetClick(final View view) {
        final String topic = "myLightBulb";
        final String msg = "g";

        try {
            mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);
            Toast.makeText(getApplicationContext(), "Disable Gadget Channel~", Toast.LENGTH_LONG).show();
            Drawable drawable= getResources().getDrawable(android.R.drawable.presence_invisible);
            GadgetStatus.setCompoundDrawablesWithIntrinsicBounds(drawable,null, null, null);
            GadgetStatus.setCompoundDrawablePadding(4);
        } catch (Exception e) {
            Log.e(LOG_TAG, "Publish error.", e);
        }
    }


    /*
     * Homekit Control("enable" and "disable")
     */
    public void enableHomekitClick(final View view) {
        final String topic = "myLightBulb";
        final String msg = "H";

        try {
            mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);
            Toast.makeText(getApplicationContext(), "Enable Homekit Channel~", Toast.LENGTH_LONG).show();
            Drawable drawable= getResources().getDrawable(android.R.drawable.presence_online);
            HomekitStatus.setCompoundDrawablesWithIntrinsicBounds(drawable,null, null, null);
            HomekitStatus.setCompoundDrawablePadding(4);
        } catch (Exception e) {
            Log.e(LOG_TAG, "Publish error.", e);
        }
    }

    public void disableHomekitClick(final View view) {
        final String topic = "myLightBulb";
        final String msg = "h";

        try {
            mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);
            Toast.makeText(getApplicationContext(), "Disable Homekit Channel~", Toast.LENGTH_LONG).show();
            Drawable drawable= getResources().getDrawable(android.R.drawable.presence_invisible);
            HomekitStatus.setCompoundDrawablesWithIntrinsicBounds(drawable,null, null, null);
            HomekitStatus.setCompoundDrawablePadding(4);
        } catch (Exception e) {
            Log.e(LOG_TAG, "Publish error.", e);
        }
    }

//    public void publishClick(final View view) {
//        final String topic = txtTopic.getText().toString();
//        final String msg = txtMessage.getText().toString();
//
//        try {
//            mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);
//        } catch (Exception e) {
//            Log.e(LOG_TAG, "Publish error.", e);
//        }
//    }

    public void disconnectClick(final View view) {
        final String topic = "myLightBulb";
        final String msg = "e";

        try {
            mqttManager.publishString(msg, topic, AWSIotMqttQos.QOS0);
            Toast.makeText(getApplicationContext(), "Exiting from AWSChannel ~", Toast.LENGTH_LONG).show();
            Drawable drawable= getResources().getDrawable(android.R.drawable.presence_invisible);
            MqttStatus.setCompoundDrawablesWithIntrinsicBounds(drawable,null, null, null);
            MqttStatus.setCompoundDrawablePadding(4);
        } catch (Exception e) {
            Log.e(LOG_TAG, "Publish error.", e);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //txtSubcribe = findViewById(R.id.txtSubcribe);
        //txtTopic = findViewById(R.id.txtTopic);
        //txtMessage = findViewById(R.id.txtMessage);

        tvLastMessage = findViewById(R.id.tvLastMessage);
        //tvClientId = findViewById(R.id.tvClientId);
        //tvStatus = findViewById(R.id.tvStatus);

        ZigbeeStatus = findViewById(R.id.textView5);
        GadgetStatus = findViewById(R.id.textView6);
        HomekitStatus = findViewById(R.id.textView7);
        MqttStatus = findViewById(R.id.textView2);

        //btnConnect = findViewById(R.id.btnConnect);
        //btnConnect.setEnabled(false);

        // MQTT client IDs are required to be unique per AWS IoT account.
        // This UUID is "practically unique" but does not _guarantee_
        // uniqueness.
        clientId = UUID.randomUUID().toString();
        //tvClientId.setText(clientId);

        // Initialize the AWS Cognito credentials provider
        AWSMobileClient.getInstance().initialize(this, new Callback<UserStateDetails>() {
            @Override
            public void onResult(UserStateDetails result) {
                initIoTClient();
                Log.d(LOG_TAG, "clientId = " + clientId);

                try {
                    mqttManager.connect(clientKeyStore, new AWSIotMqttClientStatusCallback() {
                        @Override
                        public void onStatusChanged(final AWSIotMqttClientStatus status,
                                                    final Throwable throwable) {
                            Log.d(LOG_TAG, "Status = " + String.valueOf(status));


                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    //tvStatus.setText(status.toString());
                                    Toast.makeText(getApplicationContext(), status.toString(), Toast.LENGTH_LONG).show();
                                    if(status.toString().equals("Connected"))
                                    {
                                        Drawable drawable= getResources().getDrawable(android.R.drawable.presence_online);
                                        MqttStatus.setCompoundDrawablesWithIntrinsicBounds(drawable,null, null, null);
                                        MqttStatus.setCompoundDrawablePadding(4);
                                    }
                                    if (throwable != null) {
                                        Log.e(LOG_TAG, "Connection error.", throwable);
                                    }
                                }
                            });
                        }
                    });
                } catch (final Exception e) {
                    Log.e(LOG_TAG, "Connection error.", e);
                    Toast.makeText(getApplicationContext(), "Error! " + e.getMessage(), Toast.LENGTH_LONG).show();
                    //tvStatus.setText("Error! " + e.getMessage());
                }
            }

            @Override
            public void onError(Exception e) {
                Log.e(LOG_TAG, "onError: ", e);
            }
        });
    }

    void initIoTClient() {
        Region region = Region.getRegion(MY_REGION);

        // MQTT Client
        mqttManager = new AWSIotMqttManager(clientId, CUSTOMER_SPECIFIC_ENDPOINT);

        // Set keepalive to 10 seconds.  Will recognize disconnects more quickly but will also send
        // MQTT pings every 10 seconds.
        mqttManager.setKeepAlive(10);

        // Set Last Will and Testament for MQTT.  On an unclean disconnect (loss of connection)
        // AWS IoT will publish this message to alert other clients.
        AWSIotMqttLastWillAndTestament lwt = new AWSIotMqttLastWillAndTestament("my/lwt/topic",
                "Android client lost connection", AWSIotMqttQos.QOS0);
        mqttManager.setMqttLastWillAndTestament(lwt);

        // IoT Client (for creation of certificate if needed)
        mIotAndroidClient = new AWSIotClient(AWSMobileClient.getInstance());
        mIotAndroidClient.setRegion(region);

        keystorePath = getFilesDir().getPath();
        keystoreName = KEYSTORE_NAME;
        keystorePassword = KEYSTORE_PASSWORD;
        certificateId = CERTIFICATE_ID;

        // To load cert/key from keystore on filesystem
        try {
            if (AWSIotKeystoreHelper.isKeystorePresent(keystorePath, keystoreName)) {
                if (AWSIotKeystoreHelper.keystoreContainsAlias(certificateId, keystorePath,
                        keystoreName, keystorePassword)) {
                    Log.i(LOG_TAG, "Certificate " + certificateId
                            + " found in keystore - using for MQTT.");
                    // load keystore from file into memory to pass on connection
                    clientKeyStore = AWSIotKeystoreHelper.getIotKeystore(certificateId,
                            keystorePath, keystoreName, keystorePassword);
                    /* initIoTClient is invoked from the callback passed during AWSMobileClient initialization.
                    The callback is executed on a background thread so UI update must be moved to run on UI Thread. */
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            //btnConnect.setEnabled(true);
                        }
                    });
                } else {
                    Log.i(LOG_TAG, "Key/cert " + certificateId + " not found in keystore.");
                }
            } else {
                Log.i(LOG_TAG, "Keystore " + keystorePath + "/" + keystoreName + " not found.");
            }
        } catch (Exception e) {
            Log.e(LOG_TAG, "An error occurred retrieving cert/key from keystore.", e);
        }

        if (clientKeyStore == null) {
            Log.i(LOG_TAG, "Cert/key was not found in keystore - creating new key and certificate.");

            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        // Create a new private key and certificate. This call
                        // creates both on the server and returns them to the
                        // device.
                        CreateKeysAndCertificateRequest createKeysAndCertificateRequest =
                                new CreateKeysAndCertificateRequest();
                        createKeysAndCertificateRequest.setSetAsActive(true);
                        final CreateKeysAndCertificateResult createKeysAndCertificateResult;
                        createKeysAndCertificateResult =
                                mIotAndroidClient.createKeysAndCertificate(createKeysAndCertificateRequest);
                        Log.i(LOG_TAG,
                                "Cert ID: " +
                                        createKeysAndCertificateResult.getCertificateId() +
                                        " created.");

                        // store in keystore for use in MQTT client
                        // saved as alias "default" so a new certificate isn't
                        // generated each run of this application
                        AWSIotKeystoreHelper.saveCertificateAndPrivateKey(certificateId,
                                createKeysAndCertificateResult.getCertificatePem(),
                                createKeysAndCertificateResult.getKeyPair().getPrivateKey(),
                                keystorePath, keystoreName, keystorePassword);

                        // load keystore from file into memory to pass on
                        // connection
                        clientKeyStore = AWSIotKeystoreHelper.getIotKeystore(certificateId,
                                keystorePath, keystoreName, keystorePassword);

                        // Attach a policy to the newly created certificate.
                        // This flow assumes the policy was already created in
                        // AWS IoT and we are now just attaching it to the
                        // certificate.
                        AttachPrincipalPolicyRequest policyAttachRequest =
                                new AttachPrincipalPolicyRequest();
                        policyAttachRequest.setPolicyName(AWS_IOT_POLICY_NAME);
                        policyAttachRequest.setPrincipal(createKeysAndCertificateResult
                                .getCertificateArn());
                        mIotAndroidClient.attachPrincipalPolicy(policyAttachRequest);

                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                btnConnect.setEnabled(true);
                            }
                        });
                    } catch (Exception e) {
                        Log.e(LOG_TAG,
                                "Exception occurred when generating new private key and certificate.",
                                e);
                    }
                }
            }).start();
        }
    }


}
