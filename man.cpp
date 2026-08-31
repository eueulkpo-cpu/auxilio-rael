float limitar_sensibilidade(float valor_atual, float limitee_maximo) {
    if (valor_atual > limitee_maximo ) {
        return limitee_maximo;
    }

   float sensibilidade = 00.05f;
   float limete = 00.06f;
   return valor_atual + sensibilidade * (limitee_maximo - valor_atual) * limete;

   //AIM
   18:00
using UnityEngine;

public class AimController : MonoBehaviour
{
    public Transform cameraTransform;

    [Header("Configuração")]
    public float sensitivity = 0.15f;
    public float smoothness = 12f;

    private float yaw;
    private float pitch;

    private Vector2 currentAim;
    private Vector2 targetAim;

    void Update()
    {
        if (Input.touchCount > 0)
        {
            Touch touch = Input.GetTouch(0);

            if (touch.phase == TouchPhase.Moved)
            {
                targetAim += touch.deltaPosition * sensitivity;
            }
        }

        currentAim = Vector2.Lerp(
            currentAim,
            targetAim,
            smoothness * Time.deltaTime
        );

        yaw += currentAim.x;
        pitch -= currentAim.y;

        pitch = Mathf.Clamp(pitch, -80f, 80f);

        cameraTransform.localRotation =
            Quaternion.Euler(pitch, 0f, 0f);

        transform.rotation =
            Quaternion.Euler(0f, yaw, 0f);

        targetAim = Vector2.zero;
    }
}
}