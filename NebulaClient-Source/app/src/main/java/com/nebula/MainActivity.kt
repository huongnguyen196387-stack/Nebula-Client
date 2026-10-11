package com.nebula

import android.os.Bundle
import android.view.Choreographer
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity(), Choreographer.FrameCallback {

    private lateinit var txtStatus: TextView
    private lateinit var txtFps: TextView

    private var frameCount = 0
    private var lastTimeNano = 0L
    private var currentCallbackRate = 0.0f
    private var isTracking = false

    external fun nativeInitClient(): Boolean
    external fun nativeGetStatus(): String
    external fun nativeRunOpenGlesTest(): Boolean

    companion object {
        init {
            System.loadLibrary("nebulaclient")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        txtStatus = findViewById(R.id.txtStatus)
        txtFps = findViewById(R.id.txtFps)

        val initResult = nativeInitClient()
        val statusStr = nativeGetStatus()
        txtStatus.text = "Trạng thái Native: $statusStr (Init: $initResult)"

        val glesTest = nativeRunOpenGlesTest()
        if (glesTest) {
            txtStatus.text = "${txtStatus.text} | EGL Smoke Test: Thành công"
        } else {
            txtStatus.text = "${txtStatus.text} | EGL Smoke Test: Thất bại"
        }
    }

    override fun onResume() {
        super.onResume()
        if (!isTracking) {
            isTracking = true
            lastTimeNano = 0L
            frameCount = 0
            Choreographer.getInstance().postFrameCallback(this)
        }
    }

    override fun onPause() {
        super.onPause()
        if (isTracking) {
            isTracking = false
            Choreographer.getInstance().removeFrameCallback(this)
        }
    }

    override fun doFrame(frameTimeNanos: Long) {
        if (!isTracking) return

        if (lastTimeNano == 0L) {
            lastTimeNano = frameTimeNanos
        } else {
            frameCount++
            val elapsedNanos = frameTimeNanos - lastTimeNano
            if (elapsedNanos >= 500_000_000L) {
                currentCallbackRate = (frameCount * 1_000_000_000.0f) / elapsedNanos
                frameCount = 0
                lastTimeNano = frameTimeNanos
                txtFps.text = "Nebula UI VSYNC callback rate: %.1f FPS (Không phải Minecraft FPS)".format(currentCallbackRate)
            }
        }
        Choreographer.getInstance().postFrameCallback(this)
    }
}
