using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InventoryLogic : MonoBehaviour {

    //  Public
    public ItemNameBox  itemNameBox;

    //  Bridge to other part of system
    private InventoryUI inventoryUI;

    //  System
    private Item[]      items;
    private int         itemResolution;

    //  Interaction
    List<int>           enteredIndices;
    private int         focusedIndex;
    private bool        isHolding;
    private Item        heldItem;

    public int elapsedHours;


    //  Functions
    public void InitializeLogic( int slotResolution, InventoryUI uiScript )
    {
        inventoryUI = uiScript;
        itemResolution = slotResolution;
        enteredIndices = new List<int>( 4 );
        items = new Item[ itemResolution ];
        focusedIndex = -1;
        for( int i = 0; i < itemResolution; i++ )
        {
            items[ i ] = new Item()
            {
                ammount = -1,
                id = -1,
                pickupHour = -1,
                type = -1
            };
        }
    }

    //--------------------------ITEM ADDITION-----------------------------//

    public void AddItem( int id, int type, int ammount )
    {
        int maxStack = GetMaxStack( id, type );
        if( maxStack > 1 )
            AddStackable( id, type, ammount, maxStack );
        else
            AddNonStackable( id, type );
    }

    void AddStackable( int id, int type, int ammount, int maxStack )
    {
        // Try to stack with all that are in 
        for( int i = 0; i < itemResolution; i++ )
        {
            Item item = items[ i ];
            if(item.id == id && item.type == type && item.ammount < maxStack )
            {
                item.ammount += ammount;
                if( item.ammount > maxStack )
                {
                    ammount = item.ammount - maxStack;
                    item.ammount = maxStack;
                    inventoryUI.UpdateCount( i, maxStack );
                }
                else
                {
                    inventoryUI.UpdateCount( i, item.ammount );
                    return;
                }
            }
        }
        for(int i = 0; i < itemResolution; i++ )
        {
            Item item = items[ i ];
            if(item.id < 0 )
            {
                item.id = id;
                item.type = type;
                item.ammount = ammount;
                item.pickupHour = -1;
                inventoryUI.UpdateMesh( i, id, type );
                inventoryUI.UpdateCount( i, ammount );
                return;
            }
        }
    }

    void AddNonStackable( int id, int type )
    {
        for( int i = 0; i < itemResolution; i++ )
        {
            Item item = items[ i ];
            if( item.id < 0 )
            {
                item.id = id;
                item.type = type;
                item.ammount = 1;
                item.pickupHour = -1;
                inventoryUI.UpdateMesh( i, id, type );
                inventoryUI.HideCount( i );
                return;
            }
        }
    }

    //-------------------------MOUSE INTERACTION--------------------------//

    public void EnteredSlot( int index )
    {
        enteredIndices.Add( index );
        focusedIndex = index;
        if( isHolding ) return;
        if( items[ index ].id >= 0 )
            UpdateShowedName( index );
    }

    public void ExitedSlot( int index )
    {
        enteredIndices.Remove( index );
        int count = enteredIndices.Count;

        if( isHolding )
        {
            if( count > 0 )
                focusedIndex = enteredIndices[ 0 ];
            else
                focusedIndex = -1;
            return;
        }

        for( int i = 0; i < count; i++ )
        {
            if( items[ enteredIndices[ i ] ].id >= 0 )
            {
                UpdateShowedName( enteredIndices[ i ] );
                focusedIndex = enteredIndices[ i ];
                return;
            }
        }

        itemNameBox.HideName();
        if( enteredIndices.Count > 0 )
            focusedIndex = enteredIndices[ 0 ];
        else
            focusedIndex = -1;

    }

    private void UpdateShowedName( int index )
    {
        focusedIndex = index;
        Item item = items[ index ];
        itemNameBox.SetName( ItemDataBase.Items[ item.type ][ item.id ].getInfo( elapsedHours ) );
    }

    //---------------------------ITEM PICKUP--------------------------//

    public void TryToPickItem()
    {
        if( focusedIndex < 0 ) return;
        if( isHolding )
            PickItemHolding();
         else
            PickItemFresh();
    }

    private void PickItemFresh()
    {
        if( items[ focusedIndex ].id < 0 ) return;

        heldItem = new Item( items[ focusedIndex ] );
        items[ focusedIndex ].id = -1;
        inventoryUI.HideMesh( focusedIndex );
        inventoryUI.HideCount( focusedIndex );
        inventoryUI.CopyItemToHold( focusedIndex );

        if( GetMaxStack( heldItem ) > 1 )
            inventoryUI.UpdateHeldCount( heldItem.ammount );
        else
            inventoryUI.HideHeldCount();
        SetHolding( true );
    }

    private void PickItemHolding()
    {
        Item inventoryItem = items[ focusedIndex ];
        if( inventoryItem.id == heldItem.id && inventoryItem.type == heldItem.type )
        {
            int maxStack = GetMaxStack( inventoryItem );
            if( maxStack < 2 )
                return;

            if( inventoryItem.ammount < maxStack )
            {
                int sum = inventoryItem.ammount + heldItem.ammount;
                if( sum > maxStack )
                {
                    StackItemsWithSplit( maxStack, sum );
                }
                else
                {
                    StackItemsIntoOne( sum );
                }
            }
            else
            {
                SwitchItemCountsWithHeld();
            }
        }
        else  // Switch items
        {
            if( inventoryItem.id < 0 )
            {
                PlaceItemIntoFreeSlot();
            } else
            {
                SwitchDifferentTypeItems();
            }
        }
    }

    private void SwitchItemCountsWithHeld()
    {
        Item inventoryItem = items[ focusedIndex ];
        int currentAmmount = inventoryItem.ammount;
        inventoryItem.ammount = heldItem.ammount;
        heldItem.ammount = currentAmmount;
        inventoryUI.UpdateCount( focusedIndex, inventoryItem.ammount );
        inventoryUI.UpdateHeldCount( heldItem.ammount );
    }

    private void StackItemsWithSplit( int maxStack, int sum )
    {
        items[ focusedIndex ].ammount = maxStack;
        inventoryUI.UpdateCount( focusedIndex, maxStack );
        heldItem.ammount = sum - maxStack;
        inventoryUI.UpdateHeldCount( heldItem.ammount );
    }

    private void StackItemsIntoOne( int sum )
    {
        items[ focusedIndex ].ammount = sum;
        inventoryUI.UpdateCount( focusedIndex, sum );
        inventoryUI.HideHeldMesh();
        inventoryUI.HideHeldCount();
        SetHolding( false );
    }

    private void SwitchDifferentTypeItems()
    {
        inventoryUI.SwitchItemWithHold( focusedIndex );
        Item item = new Item( items[ focusedIndex ] );

        if( GetMaxStack( heldItem ) > 1 )
            inventoryUI.UpdateCount( focusedIndex, heldItem.ammount );
        else
            inventoryUI.HideCount( focusedIndex );

        if( GetMaxStack( item ) > 1 )
            inventoryUI.UpdateHeldCount( item.ammount );
        else
            inventoryUI.HideHeldCount();

        items[ focusedIndex ].Copy( heldItem );
        heldItem.Copy( item );
    }

    private void PlaceItemIntoFreeSlot()
    {
        items[ focusedIndex ] = new Item( heldItem );
        inventoryUI.UpdateMesh( focusedIndex, heldItem.id, heldItem.type );
        inventoryUI.HideHeldCount();
        inventoryUI.HideHeldMesh();

        if( GetMaxStack( heldItem ) > 1 )
            inventoryUI.UpdateCount( focusedIndex, heldItem.ammount );
        else
            inventoryUI.HideCount( focusedIndex );

        SetHolding( false );
    }

    private void TryToPickSingle()
    {
        if( isHolding )
            PickSingleHolding();
        else
            PickSingleFresh();
    }

    private void PickSingleFresh()
    {
        Item item = items[ focusedIndex ];
        if( item.id < 0 )
            return;
        if(GetMaxStack( item ) > 1 )
        {
            if(item.ammount > 1 )
            {
                heldItem = new Item( item ) { ammount = 1 };
                inventoryUI.CopyItemToHold( focusedIndex );
                inventoryUI.UpdateHeldCount( 1 );
                SetHolding( true );

                item.ammount--;
                inventoryUI.UpdateCount( focusedIndex, item.ammount );
            } else
            {
                heldItem = new Item( item ) { ammount = 1 };
                inventoryUI.CopyItemToHold( focusedIndex );
                inventoryUI.UpdateHeldCount( 1 );
                SetHolding( true );

                item.Reset();
                inventoryUI.HideMesh( focusedIndex );
                inventoryUI.HideCount( focusedIndex );
            }
        } else
        {
            heldItem = new Item( item ) { ammount = 1 };
            inventoryUI.CopyItemToHold( focusedIndex );
            inventoryUI.HideHeldCount();
            SetHolding( true );

            item.Reset();
            inventoryUI.HideMesh( focusedIndex );
            inventoryUI.HideCount( focusedIndex );
        }
    }

    private void PickSingleHolding()
    {
        Item item = items[ focusedIndex ];
        int maxStack = GetMaxStack( heldItem );
        if( maxStack < 2 ) return;
        if(item.id == heldItem.id && item.type == heldItem.type && heldItem.ammount < maxStack )
        {
            if( item.ammount > 1 )
            {
                item.ammount--;
                heldItem.ammount++;
                inventoryUI.UpdateCount( focusedIndex, item.ammount );
                inventoryUI.UpdateHeldCount( heldItem.ammount );
            }
            else
            {
                heldItem.ammount++;
                inventoryUI.UpdateHeldCount( heldItem.ammount );
                item.Reset();
                inventoryUI.HideMesh( focusedIndex );
                inventoryUI.HideCount( focusedIndex );
            }
        }
    }

    private void SetHolding( bool newHolding )
    {
        isHolding = newHolding;
        if( isHolding )
            itemNameBox.HideName();
        else
            UpdateShowedName( focusedIndex );
    }

    private void Update()
    {
        if( Input.GetKeyDown( KeyCode.A ) )
        {
            AddItem( 0, 0, 3 );
        }else if( Input.GetKeyDown( KeyCode.Q ) )
        {
            AddItem( 0, 1, 1 );
        }
        else if( Input.GetKeyDown( KeyCode.Z ) )
        {
            AddItem( 0, 2, 1 );
        }
        else if( Input.GetMouseButtonDown(0) )
        {
            TryToPickItem();
        } else if( Input.GetMouseButtonDown(1) )
        {
            TryToPickSingle();
        }
    }

    private int GetMaxStack( Item item )
    {
        return ItemDataBase.Items[ item.type ][ item.id ].maxStack;
    }

    private int GetMaxStack( int id, int type )
    {
        return ItemDataBase.Items[ type ][ id ].maxStack;
    }
}
