using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Item
{
    public int type;
    public int id;
    public int ammount;
    public int pickupHour;

    public void Reset()
    {
        type = -1;
        id = -1;
        ammount = -1;
        pickupHour = -1;
    }
    public void Copy( Item item )
    {
        type = item.type;
        id = item.id;
        ammount = item.ammount;
        pickupHour = item.pickupHour;
    }
    public Item( Item item )
    {
        type = item.type;
        id = item.id;
        ammount = item.ammount;
        pickupHour = item.pickupHour;
    }
    public Item()
    {
        type = -1;
        id = -1;
        ammount = -1;
        pickupHour = -1;
    }
}

public static class ItemDataBase
{
    public static readonly string[] PathPrefixes =
    {
        "Materials",
        "Foods",
        "Tools",
        "Weapons",
        "Clothes"
    };
    public static readonly ItemData[][] Items =
    {
        new CraftingMaterial[]
        {
            new CraftingMaterial("UntexturedAxe", "Simple mesh", 25),
        },
        new Food[]
        {
            new Food("Food", "It's some food", 1, 1, 1, 1, 1),
        },
        new Tool[]
        {
            new Tool("Stone Axe", "A textured axe", 25, 1, 10),
        }
    };
}

public class ItemData
{
    public string name;
    public string info;
    public int maxStack;
    public virtual string getInfo( int elapsedHours ) { return name; }
}

public class CraftingMaterial : ItemData
{
    public CraftingMaterial( string name, string info, int maxStack )
    {
        this.name = name;
        this.info = info;
        this.maxStack = maxStack;
    }
    public override string getInfo( int elapsedHours )
    {
        return name + "\n" + info;
    }
}

public class Food : ItemData
{
    public int proteins;
    public int carbs;
    public int fats;
    public int maxSpoilTime;

    public Food( string name, string info, int maxStack, int proteins, int carbs, int fats, int maxSpoilTime )
    {
        this.name = name;
        this.info = info;
        this.maxStack = maxStack;
        this.proteins = proteins;
        this.carbs = carbs;
        this.fats = fats;
        this.maxSpoilTime = maxSpoilTime;
        this.info = "<color=green>" + name + "</Color>\n" +
            info +
            "\nProteins: " + proteins +
            "%\nCarbohydrates: " + carbs +
            "%\nFats: " + fats +
            "%\nQuality: ";
    }
    public override string getInfo( int elapsedHours )
    {
        return info + getQuality( elapsedHours );
    }
    public string getQuality( int elapsedHours )
    {
        float percentage = 1.0f - elapsedHours / (float)maxSpoilTime;
        if( percentage < 0.0f )
            return "<color=red>Rotting</color>";
        else if(percentage < 0.4f )
            return "<color=orange>Spoiling</color>";
        else if( percentage < 0.7f )
            return "<color=yellow>Stale</color>";
        else
            return "<color=green>Fresh</color>";
    }
}

public class Tool : ItemData
{

    public int axePower;
    public int pickaxePower;
    public int damage;

    public Tool( string name, string info, int axePower, int pickaxePower, int damage )
    {
        this.name = name;
        this.maxStack = 1;
        this.axePower = axePower;
        this.pickaxePower = pickaxePower;
        this.damage = damage;
        this.info = "<color=blue>" + name + "</Color>\n" +
            info + "\n";
        if( axePower > 0 )
            this.info += axePower + "% Axe Power\n";
        if( pickaxePower > 0 )
            this.info += pickaxePower + "% Pickaxe Power\n";
        this.info += damage + " Damage";
    }
    public override string getInfo( int elapsedHours )
    {
        return info;
    }
}

public class Weapon : ItemData
{

    public int damage;

    public Weapon( string name, string info, int damage )
    {
        this.name = name;
        this.maxStack = 1;
        this.damage = damage;
        this.info = "<color=red>" + name + "</Color>\n" +
            info + "\n" + 
            damage + " Damage";
    }
    public override string getInfo( int elapsedHours )
    {
        return info;
    }
}

public class Clothing : ItemData
{

    public int thermalProtection;
    public int damageProtection;

    public Clothing( string name, string info, int thermalProtection, int damageProtection )
    {
        this.name = name;
        this.maxStack = 1;
        this.thermalProtection = thermalProtection;
        this.damageProtection = damageProtection;
        this.info = "<color=orange>" + name + "</Color>\n" +
            info + "\n" +
            getProtection( thermalProtection ) + " Thermal Protection\n" +
            getProtection( damageProtection ) + " Damage Protection";
    }
    private string getProtection(int value)
    {
        if( value < 0 )
            return "No";
        else if( value < 25 )
            return "Weak";
        else if( value < 50 )
            return "Medium";
        else if( value < 75 )
            return "Good";
        else
            return "Very Good";
    }
    public override string getInfo( int elapsedHours )
    {
        return info;
    }
}