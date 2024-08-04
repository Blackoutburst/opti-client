package dev.blackoutburst.game.utils

import kotlinx.serialization.Serializable
import kotlinx.serialization.json.Json
import org.lwjgl.BufferUtils
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.URL
import java.nio.ByteBuffer
import java.nio.charset.StandardCharsets
import java.util.Base64

data class Skin(val name: String, val data: ByteBuffer, val model: String)

@Serializable
data class Profile(val id: String, val name: String)
@Serializable
data class Properties(val name: String, val value: String, val signature: String)
@Serializable
data class ProfileData(val id: String, val name: String, val properties: List<Properties>, val profileActions: List<String>)
@Serializable
data class ModelTexture(val SKIN: ModelSkin)
@Serializable
data class ModelSkin(val url: String, val metadata: ModelMetadata? = null)
@Serializable
data class ModelMetadata(val model: String = "classic")
@Serializable
data class ModelData(val timestamp: Long, val profileId: String, val profileName: String, val signatureRequired: Boolean, val textures: ModelTexture)

object SkinAPI {
    val json = Json { ignoreUnknownKeys = true; isLenient = true; encodeDefaults = true }
    val skins = mutableListOf<Skin>()

    fun load(name: String) {
        if (skins.find { it.name == name } != null) return

        try {
            val url = URL("https://api.mojang.com/users/profiles/minecraft/$name")
            val connection = url.openConnection()
            val input = connection.getInputStream()
            val reader = BufferedReader(InputStreamReader(input))
            val builder = StringBuilder()

            var line: String? = reader.readLine()
            while(line != null) {
                builder.append(line)
                builder.append('\n')

                line = reader.readLine()
            }

            val profile = json.decodeFromString<Profile>(builder.toString())
            val id = profile.id

            loadSkin(id)

            input.close()
            reader.close()
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    private fun decodeBase64(base64: String) {
        try {
            val decodedBytes = Base64.getDecoder().decode(base64)
            val buffer = BufferUtils.createByteBuffer(decodedBytes.size)
            buffer.put(decodedBytes)
            buffer.flip()

            val profileData = StandardCharsets.UTF_8.decode(buffer).toString();
            val modelData = Json.decodeFromString<ModelData>(profileData)

            val url = URL(modelData.textures.SKIN.url)
            val connection = url.openConnection()
            connection.connect()

            val inputStream = connection.getInputStream()
            val bytes = inputStream.readBytes()
            val byteBuffer = BufferUtils.createByteBuffer(bytes.size)
            byteBuffer.put(bytes)
            byteBuffer.flip()

            skins.add(Skin(modelData.profileName, byteBuffer, modelData.textures.SKIN.metadata?.model ?: "classic"))
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }

    private fun loadSkin(id: String) {
        try {
            val url = URL("https://sessionserver.mojang.com/session/minecraft/profile/$id?unsigned=false")
            val reader = InputStreamReader(url.openStream())
            val data = json.decodeFromString<ProfileData>(reader.readText())

            decodeBase64(data.properties[0].value)

            reader.close()
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }
}