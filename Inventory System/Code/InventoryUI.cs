using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InventoryUI : MonoBehaviour {

    private InventoryLogic inventoryLogic;

    [Header("Slots")]
    public Transform        slotsHolder;
    public ItemNameBox      itemNameBox;
    public Material         textMaterial;

    [Range( 0.01f, 1.0f )]
    public float            meshMinatureScaleMultiplier;

    [Header( "Held Item" )]
    public Transform        heldMeshTransform;
    public TextMesh         heldCountText;

    //  Private
    private GameObject[][]  itemPrefabs;
    private Transform[,]    slots;
    private Transform[,]    itemMeshRenderers;
    private float[][]       scales;
    private int             slotsWidth;
    private int             slotsHeight;
    private float           slotScale;

    // Consts
    private static readonly int AmmountOfItemTypes = ItemDataBase.Items.Length;

    //  Functions
    private void Start()
    {
        LoadItemPrefabs();
        CopyTextMaterialData();
        ParseItemRenderers();
        CalculateScales();
        inventoryLogic = GetComponent<InventoryLogic>();
        inventoryLogic.InitializeLogic( slotsHeight * slotsWidth, this );
    }

    private void CopyTextMaterialData()
    {
        Material material = slotsHolder.GetComponentInChildren<TextMesh>( true ).GetComponent<MeshRenderer>().sharedMaterial;
        textMaterial.SetTexture( "_MainTex", material.GetTexture( "_MainTex" ) );
    }

    private void LoadItemPrefabs()
    {
        int size = ItemDataBase.Items.Length;
        itemPrefabs = new GameObject[ size ][];

        for(int i = 0; i < size; i++ )
        {
            int subSize = ItemDataBase.Items[ i ].Length;
            itemPrefabs[ i ] = new GameObject[ subSize ];
            for(int j = 0; j < subSize; j++ )
            {
                itemPrefabs[ i ][ j ] = Resources.Load( "Prefabs/Items/" + ItemDataBase.PathPrefixes[ i ] + "/" + ItemDataBase.Items[ i ][ j ].name ) as GameObject;
            }
        }
    }

    private void CalculateScales()
    {
        //  Get world size of a slot
        BoxCollider2D bc = slots[ 0, 0 ].GetComponent<BoxCollider2D>();
        slotScale = bc.transform.localScale.x;
        float worldSlotSize = bc.bounds.extents.x / slotScale;
        //  Go through all prefabs and calculate their scales
        scales = new float[ AmmountOfItemTypes ][];
        for( int i = 0; i < AmmountOfItemTypes; i++ )
        {
            int ammountOfMeshes = itemPrefabs[ i ].Length;
            scales[ i ] = new float[ ammountOfMeshes ];
            for( int j = 0; j < ammountOfMeshes; j++ )
            {
                Mesh mesh = itemPrefabs[ i ][ j ].GetComponent<MeshFilter>().sharedMesh;
                Vector3 extents = Quaternion.Euler( -110f, -90f, 90f ) * mesh.bounds.extents;
                Vector3 center = mesh.bounds.center;
                float maxExtent = Mathf.Max(    Mathf.Abs( center.x ) + extents.x,
                                                Mathf.Abs( center.y ) + extents.y,
                                                Mathf.Abs( center.z ) + extents.z );
                scales[ i ][ j ] = ( worldSlotSize / maxExtent ) * meshMinatureScaleMultiplier;
            }
        }
    }

    private void ParseItemRenderers()
    {
        //  Get all children in holder
        SpriteRenderer[] childrenSpriteRenderers = slotsHolder.GetComponentsInChildren<SpriteRenderer>( true );
        Transform[] childrenTransforms = new Transform[ childrenSpriteRenderers.Length ];
        for( int i = 0; i < childrenSpriteRenderers.Length; i++ )
            childrenTransforms[ i ] = childrenSpriteRenderers[ i ].transform;
        //  Go over all children looking for only upper row ones
        float yPos = childrenTransforms[ 0 ].position.y;
        for( int i = 1; i < childrenSpriteRenderers.Length; i++ )
        {
            float currentYPos = childrenTransforms[ i ].position.y;
            if( Mathf.Abs( yPos - currentYPos ) > 0.001f )
            {
                slotsWidth = i;
                break;
            }
        }

        slotsHeight = childrenSpriteRenderers.Length / slotsWidth;
        slots = new Transform[ slotsHeight, slotsWidth ];
        itemMeshRenderers = new Transform[ slotsHeight, slotsWidth ];
        //  Sort them by x position
        for( int i = 0; i < slotsHeight; i++ )
        {
            int startingIndex = i * slotsWidth;
            float lastLowest = float.MinValue;
            for( int j = 0; j < slotsWidth; j++ )
            {
                float currentLowest = float.MaxValue;
                int lowestIndex = 0;
                for( int k = 0; k < slotsWidth; k++ )
                {
                    float x = childrenTransforms[ startingIndex + k ].position.x;
                    if( x < currentLowest && x > lastLowest )
                    {
                        currentLowest = x;
                        lowestIndex = k;
                    }
                }
                lastLowest = currentLowest;
                slots[ i, j ] = childrenTransforms[ startingIndex + lowestIndex ].transform;
                slots[ i, j ].GetComponent<InventorySlot>().myIndex = i * slotsWidth + j;
                itemMeshRenderers[ i, j ] = slots[ i, j ].GetComponentInChildren<MeshFilter>( true ).transform;
                slots[ i, j ].GetComponentInChildren<TextMesh>( true ).GetComponent<MeshRenderer>().material = textMaterial;
            }
        }
    }

    //--------------------------RUNTIME-------------------------------//

    public void UpdateCount( int index, int count )
    {
        int x = index % slotsWidth;
        int y = index / slotsWidth;
        TextMesh textMesh = slots[ y, x ].GetComponentInChildren<TextMesh>( true );
        textMesh.text = count.ToString();
        textMesh.gameObject.SetActive( true );
    }

    public void HideCount( int index )
    {
        int x = index % slotsWidth;
        int y = index / slotsWidth;
        slots[ y, x ].GetComponentInChildren<TextMesh>( true ).gameObject.SetActive( false );
    }

    public void UpdateMesh( int index, int id, int type )
    {
        int x = index % slotsWidth;
        int y = index / slotsWidth;
        itemMeshRenderers[ y, x ].GetComponent<MeshFilter>().mesh = itemPrefabs[ type ][ id ].GetComponent<MeshFilter>().sharedMesh;
        itemMeshRenderers[ y, x ].GetComponent<MeshRenderer>().materials = itemPrefabs[ type ][ id ].GetComponent<MeshRenderer>().sharedMaterials;
        itemMeshRenderers[ y, x ].localScale = Vector3.one * scales[ type ][ id ];
        itemMeshRenderers[ y, x ].gameObject.SetActive( true );
    }

    public void HideMesh( int index )
    {
        int x = index % slotsWidth;
        int y = index / slotsWidth;
        itemMeshRenderers[ y, x ].gameObject.SetActive( false );
    }

    public void CopyItemToHold( int index )
    {
        int x = index % slotsWidth;
        int y = index / slotsWidth;
        heldMeshTransform.GetComponent<MeshRenderer>().materials = itemMeshRenderers[ y, x ].GetComponent<MeshRenderer>().materials;
        heldMeshTransform.GetComponent<MeshFilter>().mesh = itemMeshRenderers[ y, x ].GetComponent<MeshFilter>().mesh;
        heldMeshTransform.localScale = itemMeshRenderers[ y, x ].localScale * slotScale;
        heldMeshTransform.gameObject.SetActive( true );
    }

    public void HideHeldMesh()
    {
        heldMeshTransform.gameObject.SetActive( false );
    }

    public void HideHeldCount()
    {
        heldCountText.gameObject.SetActive( false );
    }

    public void UpdateHeldCount( int count )
    {
        heldCountText.gameObject.SetActive( true );
        heldCountText.text = count.ToString();
    }

    public void SwitchItemWithHold( int index )
    {
        int x = index % slotsWidth;
        int y = index / slotsWidth;

        MeshRenderer heldRenderer = heldMeshTransform.GetComponent<MeshRenderer>();
        MeshFilter heldFilter = heldMeshTransform.GetComponent<MeshFilter>();
        float heldScale = heldRenderer.transform.localScale.x;

        Material[] materials = heldRenderer.materials;
        Mesh mesh = heldFilter.mesh;

        MeshRenderer itemRenderer = itemMeshRenderers[ y, x ].GetComponent<MeshRenderer>();
        MeshFilter itemFilter = itemMeshRenderers[ y, x ].GetComponent<MeshFilter>();
        heldRenderer.transform.localScale = itemRenderer.transform.localScale * slotScale;
        itemRenderer.transform.localScale = Vector3.one * heldScale / slotScale;

        heldRenderer.materials = itemRenderer.materials;
        heldFilter.mesh = itemFilter.mesh;
        itemRenderer.materials = materials;
        itemFilter.mesh = mesh;
    }

}