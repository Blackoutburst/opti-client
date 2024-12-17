package dev.blackoutburst.game.utils

import java.io.File
import java.io.FileNotFoundException
import java.io.FileOutputStream

fun extractDll(resourcePath: String): String {
    val inputStream = object {}.javaClass.getResourceAsStream("/$resourcePath")
        ?: throw FileNotFoundException("DLL not found: $resourcePath")
    val tempFile = File.createTempFile("DesktopWindow", ".dll")
    tempFile.deleteOnExit()
    inputStream.use { input ->
        FileOutputStream(tempFile).use { output ->
            input.copyTo(output)
        }
    }
    return tempFile.absolutePath
}