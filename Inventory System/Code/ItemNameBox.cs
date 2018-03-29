using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ItemNameBox : MonoBehaviour
{
    public Transform        nameTransfrom;
    public Transform        backgroundTransform;
    public Material         textMaterial;
    public float            offsetFromMouse;
    public float            boxSizeMultiplier;

    private Renderer        nameRenderer;
    private TextMesh        nameTextMesh;
    private float           scaleMultiplier;
    private float           nameLocalScale;

    private void Start()
    {
        //  Generate normalized quad
        Vector3[] vertices =
        {
            new Vector3(-1f, -1f, 0f ),
            new Vector3(-1f,  1f, 0f ),
            new Vector3( 1f,  1f, 0f ),
            new Vector3( 1f, -1f, 0f )
        };
        int[] triangles =
        {
            0, 1, 3,
            3, 1, 2
        };
        Vector2[] uvs =
        {
            new Vector2( 0f, 0f ),
            new Vector2( 0f, 1f ),
            new Vector2( 1f, 1f ),
            new Vector2( 1f, 0f )
        };
        Mesh quadMesh = new Mesh();
        quadMesh.vertices = vertices;
        quadMesh.triangles = triangles;
        quadMesh.uv = uvs;
        quadMesh.UploadMeshData( true );
        backgroundTransform.GetComponent<MeshFilter>().mesh = quadMesh;

        //  Cache
        nameRenderer = nameTransfrom.GetComponent<Renderer>();
        nameTextMesh = nameTransfrom.GetComponent<TextMesh>();
        nameLocalScale = nameTransfrom.localScale.x;
        scaleMultiplier = boxSizeMultiplier / nameLocalScale;

        
        nameRenderer.material = textMaterial;
    }

    public void SetName( string name )
    {
        nameTransfrom.gameObject.SetActive( true );
        nameTextMesh.text = name;
        Vector3 extents = nameRenderer.bounds.extents * scaleMultiplier;
        backgroundTransform.localScale = new Vector3( extents.x, extents.y, 1f );
        extents.x += offsetFromMouse;
        extents.y += offsetFromMouse;
        extents.y *= -1.0f;
        nameTransfrom.localPosition = extents * nameLocalScale;
    }

    public void HideName()
    {
        nameTransfrom.gameObject.SetActive( false );
    }

}