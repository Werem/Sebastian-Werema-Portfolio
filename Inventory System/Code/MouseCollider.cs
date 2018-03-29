using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MouseCollider : MonoBehaviour {
    
    public InventoryLogic   inventoryLogic;

    private Transform myTransform;
    private Camera cam;

    private void Start()
    {
        myTransform = transform;
        cam = Camera.main;
    }

    private void Update()
    {
        Vector3 mousePosition = Input.mousePosition;
        mousePosition.z = 0f;
        mousePosition = cam.ScreenToWorldPoint( mousePosition );
        mousePosition.z = 0f;
        myTransform.position = mousePosition;
    }

    private void OnCollisionEnter2D( Collision2D collision )
    {
        Collider2D collider = collision.collider;
        string name = collider.name;
        switch( name[ 0 ] )
        {
            case 'S':
                EvaluateSlotOn( ref collider );
                return;
                
        }
    }

    private void OnCollisionExit2D( Collision2D collision )
    {
        Collider2D collider = collision.collider;
        string name = collider.name;
        switch( name[ 0 ] )
        {
            case 'S':
                EvaluateSlotOff( ref collider );
                return;
        }
    }

    private void EvaluateSlotOn( ref Collider2D collider )
    {
        inventoryLogic.EnteredSlot( collider.GetComponent<InventorySlot>().myIndex );   
    }

    private void EvaluateSlotOff( ref Collider2D collider )
    {
        inventoryLogic.ExitedSlot( collider.GetComponent<InventorySlot>().myIndex );
    }
}